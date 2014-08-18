#include <OneWire.h>             // Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h>   // Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <LiquidCrystal.h>       // Taken from the Arduino website
#include <Pitches.h>             // Taken from the Arduino website

/********** SENSORS **********/
// DS18B20 Pinout (Left to Right, pins down, flat side toward you)
//  - Left   = Ground
//  - Center = Signal (with 3.3K to 4.7K resistor to +5 or +3.3 )
//  - Right  = +5 or +3.3 V
// Note: When chained together, all Signal pins parallel on same wire. Resistor still
// connected b/t Signal and +5 as noted above. All ground and +5 pins both hooked up to
// Ground line (nothing on +5 line, other than resistor connected to Signal line).

// See DS18B20_find_address sketch to find these:
// Sensor A: 0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE
// Sensor B: 0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58

// Define data pin for OneWire bus (all sensors parallel on this pin)
#define ONE_WIRE_BUS_PIN 12
// Instantiate OneWire object
OneWire ds(ONE_WIRE_BUS_PIN);

// Pass our OneWire reference to Dallas Temperature.
DallasTemperature sensors(&ds);

// See "DS18B20_find_address" sketch to find these. Each sensor will have their own address that needs
// to be declared here.
DeviceAddress SensorA = { 
  0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE 
}; 
DeviceAddress SensorB = { 
  0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58 
};

/********** RGB LED **********/
/*Note: Radio Shack sells common anode versions. So, with the flat side to the left, the pin-out is as follows:
    ______
   |      |
   |      |
  _|      |_
 |__________)
   | |  | |
   | |  | |
   | |  | |
   | |  | |
   | |  | |
   R A* B G
   E    L R
   D    U E
        E E
          N
* A* = Common Anode to +5V
* Red: 220 Ohm resistor,then to pin
* Blue/Green: 100 Ohm resistor,then to pin
* All 3 pins must be on a PWM pin to work (denoted by a '~' next to their pin number on the board   
*/
#define RED_PIN 11
#define GREEN_PIN 10
#define BLUE_PIN 9

// Vars to store temps read from sensors
float temp01;
float temp02;

/********** LCD DISPLAY **********/
// Pin name on LCD:     { VSS, VDD, VO,        RS, RW,  E, D0, D1, D2, D3, D4, D5, D6, D7, A,   K   }
// Pin name on Arduino: { Gnd, Vcc, *resistor, 2,  Gnd, 3, NA, NA, NA, NA, 4,  5,  6,  7,  Vcc, Grd }
// *I forget which resistor we would up using to make the screen contrast work. Would have to experiment -__-

// Initialize LCD Display object
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

/**************** BUZZER ****************/
// Hook Ground on buzzer to Ground, other one to your pin on the board; no resistor necessary
#define buzzerPin 8
int notes[] = {  // Notes in the melody:
  NOTE_C8
};

int notesSize = sizeof(notes) / sizeof(int);

int notesLength[] = {  // Note durations: 4 = quarter note, 8 = eighth note, etc.:
  4
};

int tempTarget01 = 90;

// Var to store "room temp" for use in calculation of indicator intervals
#define ROOM_TEMP 80

// Var to store indicator interval step sizes
int indicatorStep;

void setup() 
{
  Serial.begin(9600);
  // Fire up LCD (16 chars wide, 2 rows deep)
  lcd.begin(16, 2);

  // Fire up the sensors
  sensors.begin();
  // Set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(SensorA, 10);
  sensors.setResolution(SensorB, 10); 

  // Declare pin modes
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.print("Initializing Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());
  Serial.println("");
  
  // This presumes 3 LED states: > tempTarget01 = red; > indicatorStep = green; else blue.
  // Would have to change this formula if more indication steps are needed.
  indicatorStep = (tempTarget01 - ROOM_TEMP) / 2;
  
  // Print boot message
  lcd.clear();
  //lcd.setCursor(0, 0);
  lcd.print("*** Booting ***");
  setColor(0, 255, 0);
  delay(2000);
  lcd.clear();
  
  Serial.print("tempTarget01: ");
  Serial.print(tempTarget01);
  Serial.println(" *F");
  Serial.println("");
  
  Serial.print("ROOM_TEMP: ");
  Serial.print(ROOM_TEMP);
  Serial.println(" *F");
  
  // DEBUG: Keep only for initial debugging; delete once not needed
  Serial.print("indicatorStep: ");
  Serial.println(indicatorStep);

  // Print tempTargets to LCD
  lcd.print("Target 1:  ");
  lcd.print(tempTarget01);
  delay(5000);
  lcd.clear();
  
  Serial.println("Getting temperatures... ");
  Serial.println("");

}// End setup()

void loop() 
{
  // Command all devices on bus to read temperature  
  sensors.requestTemperatures();
  temp01 = readTemp(SensorA);
  temp02 = readTemp(SensorB);

  // Print temps to Serial & LCD
  printTemps();
  
  // Test temp and indicate (LED & buzzer) accordingly
  indicate(temp01);
  
  delay(500);
}// End loop()




/********** USER DEFINED METHODS **********/

/** 
 * Read temperatures from sensors.
 * @param the address of the sensor to be read.
 * @return the temperature, in Ferenheit.
 */
float readTemp(DeviceAddress deviceAddress)
{
  float tempF = DallasTemperature::toFahrenheit(sensors.getTempC(deviceAddress));
  return tempF;
} // END

/**
 * Print temperatures to Serial & LCD.
 */
void printTemps()
{
  // Print temps to Serial
  Serial.print("Tank 1:   ");
  Serial.print(temp01);
  Serial.println(" *F");

  Serial.print("Tank 2:   ");
  Serial.print(temp02);
  Serial.println(" *F");
  Serial.println("");

  // Print temps to LCD
  lcd.setCursor(0, 0);
  lcd.print("Tank 1: ");
  lcd.print(temp01);
  lcd.print(" *F");
  lcd.setCursor(0, 1);

  lcd.print("Tank 2: ");
  lcd.print(temp02);
  lcd.print(" *F");
} // END

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
} // END

/** 
 * Test the temperature and indicate accordingly.
 * @param temperature.
 */
void indicate(float temp)
{
  //float readTemp = temp;
  int step1 = ROOM_TEMP + indicatorStep;
  
  if (temp > (tempTarget01))
  {
    setColor(0, 255, 0); // Green
    for (int i = 0; i < notesSize ; i++) 
    {
      int notesDuration = 1000/notesLength[i];
      tone(buzzerPin, notes[i], notesDuration);
    }
  }
  else if ( temp > step1 )
  {
    setColor(255, 255, 0); // Yellow
  }
  else
  {
    setColor(255, 0, 0); // Red
  }
}// END
