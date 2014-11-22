/* Tester for touchscreen */

/************************** INCLUDE DEPENDANCIES ******************************/
#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>

#include <OneWire.h>             // Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h>   // Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library\

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 140
#define TS_MAXX 900

#define TS_MINY 120
#define TS_MAXY 940

/**************************** TEMP SENSORS ************************************/
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

/*********************** VARIABLES/INIT OBJECTS *******************************/
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Flag to keep track of current menu
int curMenu;

// Variables to store temps read from sensors
float tempBoil;
float tempMash;

/****************************** END SETUP *************************************/

void setup(void) 
{
  Serial.begin(9600); // Init Serial comm

  Tft.init();  //init TFT library
  Tft.paintScreenBlack();  // Clear screen
  Tft.setDisplayDirect(UP2DOWN); // Orient display as landscape

  // Draw welcome message
  //Tft.drawString("brewbot v.1", 50, 50, 2, WHITE);
  //delay(10000);

  // Set and draw main menu
  curMenu = 0;
  printMenu(curMenu);
  Serial.println("Main menu");
  Serial.print("curMenu = "); Serial.println(curMenu);
  Serial.println("");

  // Fire up the temp sensors
  sensors.begin();
  // Set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(SensorA, 10);
  sensors.setResolution(SensorB, 10); 
  Serial.print("Initializing Temperature Control Library Version: ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.print("Number of Devices found on bus: ");  
  Serial.println(sensors.getDeviceCount());
  Serial.println("");

  Serial.println("Test temperatures... ");
  sensors.requestTemperatures();
  tempBoil = readTemp(SensorA);
  tempMash = readTemp(SensorB);
  Serial.print("tempBoil: "); Serial.println(tempBoil);
  Serial.print("tempMash: "); Serial.println(tempMash);
  Serial.println("");
}

void loop(void) 
{
  // a point object holds x y and z coordinates
  Point p = ts.getPoint();

  // Check to see if any buttons have been pressed
  if (p.z > ts.pressureThreshhold)
  {
    p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
    Serial.print("x = "); Serial.print(p.x);
    Serial.print("\ty = "); Serial.print(p.y);
    Serial.print("\tz = "); Serial.println(p.z);

    // Check to see if a button was pressed
    if (curMenu == 0) // Main Menu
    {
      if (p.y >= 90 && p.y <= 230)
      {
        if (p.x >= 140 && p.x <= 200)
        {
          curMenu = 1; // H2O menu
          printMenu(curMenu);
          Serial.println("H2O");
          Serial.println("");

          // Used to 'clear out' the old p to prevent 'double clicking' 
          // on the button on the next generated screen. p gets reset to
          // a valid point at the top of the loop so the next touch can
          // be read. I couldn't find a way around using this 'hack'.
          p = ts.getPoint(); 
        }
        if (p.x >= 40 && p.x <= 100)
        {
          curMenu = 2; // Start menu
          printMenu(curMenu);
          Serial.println("Start");
          Serial.println("");
          p = ts.getPoint(); 
        }
      }
    }
    if (curMenu == 1) // H2O Menu
    {
      if (p.y >= 10 && p.y <= 246)
      {
        if (p.x >= 184 && p.x <= 224)
        {
          curMenu = 3;
          printMenu(curMenu);
          Serial.println("Pale Ale");
          Serial.println("");
        }
        if (p.x >= 128 && p.x <= 168)
        {
          curMenu = 4;
          printMenu(curMenu);
          Serial.println("Amber/ESB");
          Serial.println("");
        }
        if (p.x >= 72 && p.x <= 112)
        {
          curMenu = 5;
          printMenu(curMenu);
          Serial.println("Brown/Porter");
          Serial.println("");
        }
        if (p.x >= 16 && p.x <= 56)
        {
          curMenu = 6;
          printMenu(curMenu);
          Serial.println("Stout");
          Serial.println("");
        }
      }
      if (p.y >= 254 && p.y <= 314)
      {
        if (p.x >= 16 && p.x <= 224)
        {
          curMenu = 0;
          printMenu(curMenu);
          Serial.println("Back");
          Serial.println("");
        }
      }
    }
    if (curMenu == 2 || curMenu == 3 || curMenu == 4 || curMenu == 5 || curMenu == 6)
    {
      if (p.y >= 254 && p.y <= 314)
      {
        if (p.x >= 16 && p.x <= 224)
        {
          curMenu = 1;
          printMenu(curMenu);
          Serial.println("Back");
          Serial.println("");
        }
      }
    }
  }

  if (curMenu == 2) // Start
  {
    
  }
  delay(100);
}

/********************* USER DEFINED FUNCTIONS *********************************/
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
 * Print temperatures to Serial & screen.
 */
void printTemps()
{
  // Print temps to Serial
  Serial.print("Boil Temp: ");
  Serial.print(tempBoil);
  Serial.println(" *F");

  Serial.print("Mash Temp: ");
  Serial.print(tempMash);
  Serial.println(" *F");
  Serial.println("");

  // Test print; need to dtostr the floats first
  Tft.drawString("tempA", 180, 125, 2, WHITE);
  Tft.drawString("tempB", 80, 125, 2, WHITE);
}

void printMenu(int m)
{
  if (m == 0)
  {
    Tft.paintScreenBlack();
    Tft.fillRectangle(140, 90, 60, 140, BLUE); // {y,x,h,w,color}
    Tft.drawString("H2O", 180, 125, 3, WHITE); // {y,x,font-size,color}
    Tft.fillRectangle(40, 90, 60, 140, GREEN);
    Tft.drawString("Start", 80, 105, 3, BLACK);
  }
  if (m == 1)
  {
    Tft.paintScreenBlack();
    Tft.fillRectangle(184, 10, 40, 236, BLUE);
    Tft.drawString("Pale Ale", 204, 20, 2, WHITE); 
    
    Tft.fillRectangle(128, 10, 40, 236, BLUE);
    Tft.drawString("Amber/ESB", 148, 20, 2, WHITE);

    Tft.fillRectangle(72, 10, 40, 236, BLUE);
    Tft.drawString("Brown/Porter", 92, 20, 2, WHITE);

    Tft.fillRectangle(16, 10, 40, 236, BLUE);
    Tft.drawString("Stout", 36, 20, 2, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60, 274, 3, WHITE);
  }
  if (m == 2)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Mash:", 210, 16, 3, WHITE);
    Tft.drawString("Boil:", 160, 16, 3, WHITE);
    Tft.drawString("Time:", 40, 16, 3, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60, 274, 3, WHITE);
  }
  if (m == 3)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Pale Ale    5gal   10gal", 210, 10, 1, RED);
    Tft.drawString("Dilution:    50%     50%", 180, 10, 1, WHITE);
    Tft.drawString("Gypsum:     7.0g   14.0g", 150, 10, 1, WHITE);
    Tft.drawString("Salt:       1.2g    2.4g", 120, 10, 1, WHITE);
    Tft.drawString("CaCl2:      1.0g    2.0g", 90, 10, 1, WHITE);
    Tft.drawString("Pick Lime:  0.2g    0.4g", 60, 10, 1, WHITE);
    Tft.drawString("MgCl2:      1.0g    2.0g", 30, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
  if (m == 4)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Amber/ESB    5gal   10gal", 210, 10, 1, RED);
    Tft.drawString("Dilution:      0%      0%", 180, 10, 1, WHITE);
    Tft.drawString("Gypsum:     1.80g   3.60g", 150, 10, 1, WHITE);
    Tft.drawString("Salt:       0.00g   0.00g", 120, 10, 1, WHITE);
    Tft.drawString("CaCl2:      1.20g   2.40g", 90, 10, 1, WHITE);
    Tft.drawString("Phos Acid:  0.80g   1.60g", 60, 10, 1, WHITE);
    Tft.drawString("MgCl2:      1.12g   2.24g", 30, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
  if (m == 5)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Brwn/Prtr    5gal   10gal", 210, 10, 1, RED);
    Tft.drawString("Dilution:     50%     50%", 180, 10, 1, WHITE);
    Tft.drawString("Gypsum:     1.80g   3.60g", 150, 10, 1, WHITE);
    Tft.drawString("Pick Lime:  0.40g   0.80g", 120, 10, 1, WHITE);
    Tft.drawString("CaCl2:      0.80g   1.60g", 90, 10, 1, WHITE);
    Tft.drawString("Phos Acid:  0.80g   1.60g", 60, 10, 1, WHITE);
    Tft.drawString("MgCl2:      1.12g   2.24g", 30, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
  if (m == 6)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Stout        5gal   10gal", 225, 10, 1, RED);
    Tft.drawString("Dilution:     25%     25%", 205, 10, 1, WHITE);
    Tft.drawString("Gypsum:      0.8g   1.6g", 175, 10, 1, WHITE);
    Tft.drawString("Salt:        0.4g   0.8g", 145, 10, 1, WHITE);
    Tft.drawString("CaCl2:       0.4g   0.8g", 115, 10, 1, WHITE);
    Tft.drawString("Phos Acid:   1.0g   2.0g", 85, 10, 1, WHITE);
    Tft.drawString("Pick Lime:   0.8g   1.6g", 55, 10, 1, WHITE);
    Tft.drawString("MgCl2:       1.0g   2.0g", 25, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
}
