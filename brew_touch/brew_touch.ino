/*
TouchScreenMenu Library, 2012 Copyright (c) Scott Shaver
 */
#include <TouchScreenMenu.h>     // https://code.google.com/p/touch-screen-menu-library/
#include <TouchScreen.h>         // https://github.com/adafruit/Touch-Screen-Library
#include <TFT.h>
#include <cstddef.h>
#include <stdint.h>
#include <OneWire.h>             // Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h>   // Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <Pitches.h>             // Taken from the Arduino website

/******************* MENU SETUP ***********************/
// create the array of items for the main menu
TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("Water Chem"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the first sub menu
TouchScreenMenuItem waterItems[] = {
  TouchScreenMenuItem("Back"),
  TouchScreenMenuItem("Pale Ale"),
  TouchScreenMenuItem("Amber/ESB"),
  TouchScreenMenuItem("Brown/Porter"),
  TouchScreenMenuItem("Stout"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the various menus setting the items, font size, spacing, padding, justification and titles
TouchScreenMenu waterMenu = TouchScreenMenu(waterItems, 2, 15, 10, CENTERJ, "Water Chem");
TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 20, 20, CENTERJ, "Main Menu");

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &mainMenu;

// Buttons for non-menu screens that are only monitored if curMenu=NULL (i.e. not on a menu screen)
TouchScreenArea backBtn =  TouchScreenButton("Back",  WHITE, BLACK, 20, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea resetBtn = TouchScreenButton("Reset", WHITE, BLACK, 125, TSC.getScreenHeight() - 50, 2, 10);

/******************* TIMER SETUP *************************/
long interval = 1000;  // Threshold at which to update the Timer
unsigned long currentMillis = 0;  // current millis
unsigned long previousMillis = 0;  // Will store last time Timer was updated
unsigned long benchMillis = 0;  // Variable used to be able to reset second to zero and still be able to keep track of time
int second = 0, minute = 0, hour = 0; // Keep track of time so we can increment min/hours based on elapsed seconds
const long MILLIS_IN_MINUTE = 60000;  // Constant to calculate printable time
char _hour[3]; // Buffer to store the current hour in. Used to compare to see if it has changed and needs to be updated. Prevents blinking every pass through main loop.
char _minute[3];
char _second[3];
boolean displaySensorSwitch = false;

/********** SENSORS **********/
// DS18B20 Pinout (Left to Right, pins down, flat side toward you)
//  - Left   = Ground
//  - Center = Signal (with 3.3K to 4.7K resistor to +5 or +3.3 )
//  - Right  = +5 or +3.3 V
// Note: When chained together, all Signal pins parallel on same wire. Resistor still
// connected b/t Signal and +5 as noted above. Ground and +5 pins both hooked up to
// Ground line (nothing on +5 line, other than resistor connected to Signal line

// See DS18B20_find_address sketch to find these:
// Sensor A: 0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE
// Sensor B: 0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58

// Define data pin for OneWire bus (all sensors parallel on this pin)
#define ONE_WIRE_BUS_PIN 22
// Instantiate OneWire object
OneWire ds(ONE_WIRE_BUS_PIN);

// Pass our OneWire reference to Dallas Temperature.
DallasTemperature sensors(&ds);

// See DS18B20_find_address sketch to find these
DeviceAddress SensorA = { 
  0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE 
}; 
DeviceAddress SensorB = { 
  0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58 
};

// Sensor stuff
float tempUpper = 0;  // var to store temp reading for upper pot
float tempMash = 0;   // var to store temp reading of mash tun
char textUpper[8];    // buffer to store the results of dtostrf
char textMash[8];     // buffer to store the results of dtostrf

/* TODO: These vars were here for use with the physical switch to select 5/10 gallon
 batches. Consider adding a software switch to select batch size. If not, refactor
 these out and simplify program */
// Var to store which boil target, depending on size of batch (10 vs. 5 gallons).
int boilTarget = 86;
int batchSize = 5;
// Var to store "room temp" for use in calculation of indicator intervals
#define ROOM_TEMP 80
/* This presumes 3 LED states: > boilTarget = red; > indicatorStep = green; else blue.
 Would have to change this formula if more indication steps are needed. */
int indicatorStep = (boilTarget - ROOM_TEMP) / 2;

/********** RGB LED **********/
// See LED_RGB sketch for pinout and resistor documentation
// TODO: These need to be PWM pins on the Mega
#define RED_PIN 23
#define GREEN_PIN 24
#define BLUE_PIN 25

/**************** BUZZER ****************/
// Hook Grd on buzzer to Grd, other to pin; no resistor used
#define BUZZER_PIN 26
int notes[] = {  // Notes in the melody:
  NOTE_C8
};

int notesSize = sizeof(notes) / sizeof(int);

int notesLength[] = {  // Note durations: 4 = quarter note, 8 = eighth note, etc.:
  4
};

// Set up everything
void setup(void) {
  //Serial.begin(9600);
  //Serial.println("Booting...");
  
  // Fire up the sensors
  sensors.begin();
  // Set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(SensorA, 10);
  sensors.setResolution(SensorB, 10);

  /*Serial.print("Initializing Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());
  Serial.println("");*/

  // Declare pin modes for LED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Setup clock
  previousMillis = millis();
  
  // Change the default background color
  TSC.setBackColor(BLACK);
  // Make sure everything get initialized
  TSC.init();
  // Put up the main menu
  curMenu->draw();
}

void loop(void) {
  //Serial.println("Enter loop"); // DEBUG
  // Update the time
  updateTimer();
  if (displaySensorSwitch == true) {
    displaySensor();
    displayTimer();
    clearDisplay();
  }
  
  // handle the current menu
  if(curMenu!=NULL){
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
  }else{
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them were pressed
    checkButtons();
  }
  //Serial.println("Exit loop"); // DEBUG
  //Serial.println(""); // DEBUG
}

// check to see if any menu item was pressed and do something
void checkMenuSelection(TouchScreenMenuItem *item) {
  //Serial.println("Enter checkMenuSelection()"); // DEBUG
  boolean handled = false;
  if(item != NULL){
    // main menu items 
    if(curMenu == &mainMenu){
      if(!strcmp(item->getText(),"Water Chem")){
        curMenu = &waterMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      // Main Display
      else if(!strcmp(item->getText(),"Start")){
        curMenu = NULL;
        TSC.clearScreen();
        TSC.drawString("Upper:", 20, 20, 2, WHITE);
        TSC.drawString("Mash:", 20, 50, 2, WHITE);
        TSC.drawString(":", 50, 80, 2, WHITE);
        TSC.drawString(":", 95, 80, 2, WHITE);
        
        // Turn on timer display in main()
        displaySensorSwitch = true;
        
        // Draw buttons
        backBtn.draw();
        resetBtn.draw();
        handled = true;
      }
    }
    else if(curMenu == &waterMenu){
      if(!strcmp(item->getText(),"Back")){
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Pale Ale")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Pale Ale    5gal  10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:    50%    50%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     7.0g  14.0g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Salt:       1.2g   2.4g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Pick Lime:  0.2g   0.4g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Amber/ESB")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Amber/ESB    5gal   10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:      0%      0%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     1.80g   3.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Salt:       0.00g   0.00g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      1.20g   2.40g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Phos Acid:  0.80g   1.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.12g   2.24g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Brown/Porter")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Brn/Prtr     5gal   10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:     50%     50%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     1.80g   3.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Pick Lime:  0.40g   0.80g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      0.80g   1.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Phos Acid:  0.80g   1.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.12g   2.24g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Stout")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Stout       5gal  10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:    25%    25%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     0.8g   1.6g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Salt:       0.4g   0.8g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      0.4g   0.8g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Phos Acid:  1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Pick Lime:  0.8g   1.6g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
    }
    
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
      curMenu->drawItem(item,false);
    }
  //Serial.println("Exit checkMenuSelection()"); // DEBUG
}

// check various buttons and perform actions if any was pressed
void checkButtons(){
  //Serial.println("Enter checkButtons()"); // DEBUG
  if(backBtn.process()){
    displaySensorSwitch = false;
    curMenu = &mainMenu;
    TSC.clearScreen();
    curMenu->draw();
  }
  else if(resetBtn.process()){
    // Reset timer
  }
  //Serial.println("Exit checkButtons()"); // DEBUG
}

// Update the timer displayed on "Start" screen
void updateTimer()
{
  //Serial.println("Enter updateTimer()"); // DEBUG
  // Check to see if 1 second has elapsed
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    // Save the last time you updated Timer
    previousMillis = currentMillis;
    
    /*** UPDATE TIMER COMPONENTS ***/
    if (currentMillis > benchMillis + MILLIS_IN_MINUTE) 
    {
      minute++;
      second = 0;
      benchMillis = currentMillis;
    }
    else
    {
      second = (currentMillis - benchMillis) / 1000;
    }
    if (minute > 59) 
    {
      hour++;
      minute = 0;
    }
  }
  //Serial.println("Exit updateTimer()"); // DEBUG
}

// Contert h/m/s into strings and print to display
void displayTimer()
{
  //Serial.println("Enter displayTimer()"); // DEBUG
  // Print ints to string in preparation of printing to display
  sprintf(_hour, "%02d", hour); // Arguments are: (buffer, format, value to print)
  //Serial.print(_hour);
  //Serial.print(":");
  
  sprintf(_minute, "%02d", minute); 
  //Serial.print(_minute);
  //Serial.print(":");  
  
  sprintf(_second, "%02d", second);
  //Serial.println(_second);
  
  // Display text
  TSC.drawString(_hour, 20, 80, 2, WHITE);
  TSC.drawString(_minute, 65, 80, 2, WHITE);
  TSC.drawString(_second, 110, 80, 2, WHITE);
  
  //Serial.println("Exit displayTimer()"); // DEBUG
}

void displaySensor()
{
  // Read sensors and update tempUpper & tempMash
  // Command all devices on bus to read temperature  
  sensors.requestTemperatures();
  tempUpper = readTemp(SensorA);
  tempMash = readTemp(SensorB);
  
  // Convert sensor data (float) to strings for display.
  dtostrf(tempUpper, 1, 2, textUpper);  // Arguments are (float, width, precision, buffer)
  dtostrf(tempMash, 1, 2, textMash);
  
  // Display results
  TSC.drawString(textUpper, 150, 20, 2, WHITE);
  //Serial.print("textUpper: ");  // DEBUG
  //Serial.println(textUpper);  // DEBUG

  TSC.drawString(textMash, 150, 50, 2, WHITE);
  //Serial.print("textMash: ");  // DEBUG
  //Serial.println(textMash);  // DEBUG
}

void clearDisplay()
{
  // Clear sensor
  TSC.drawString(textUpper, 150, 20, 2, BLACK);
  TSC.drawString(textMash, 150, 50, 2, BLACK);
  
  // Clear timer
  TSC.drawString(_hour, 20, 80, 2, BLACK);
  TSC.drawString(_minute, 65, 80, 2, BLACK);
  TSC.drawString(_second, 110, 80, 2, BLACK);
}

/** 
 * Read temperatures from sensors.
 * @param the address of the sensor to be read.
 * @return the temperature, in Ferenheit.
 */
float readTemp(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  float tempF = DallasTemperature::toFahrenheit(tempC);
  return tempF;
}

/** 
 * Set the LED to a particular RGB value.
 * @param red, green and blue values.
 */
void setColor(int red, int green, int blue)
{
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

/** 
 * Test the temperature and indicate accordingly.
 * @param temperature.
 */
void indicate(float temp)
{
  float readTemp = temp;
  int step1 = ROOM_TEMP + indicatorStep;
  
  if (readTemp > boilTarget)
  {
    setColor(255, 0, 0);
    for (int i = 0; i < notesSize ; i++) 
    {
      int notesDuration = 1000/notesLength[i];
      tone(BUZZER_PIN, notes[i], notesDuration);
    }
  }
  else if ( readTemp > step1 )
  {
    setColor(0, 255, 0);
  }
  else
  {
    setColor(0, 0, 255);
  }
}
