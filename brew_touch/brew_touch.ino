/********************************************************************
 * Read and display data from two temp sensors on a touchscreen.
 *
 * Seeedstudio LCD 2.8" TFT touchscreen
 * 2 X DS18B20 waterproof temp sensors
 * Arduino Mega
 *******************************************************************/

#include <stdint.h>
#include <TouchScreen.h> // Will be used to read touch events
#include <OneWire.h> // Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h> //Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <math.h>

/********************************************************************
* Set up touch screen for use w/ Mega
********************************************************************/
#include <TFT.h> // In this file, line #33 must be changed to either MEGA or SEEEDUINO depending on which board is being used

#ifdef MEGA
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3
 
#else           // Used for Uno
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3 

#endif

/********************************************************************
* Set up oneWire sensors and pass them to the Dallas Temp library
 
 * Connections for sensors:
 * DS18B20 Pinout:
 * - Black   = Ground
 * - Orange  = Signal (with 4.7K pull-up resistor to +5V )
 * - Red     = +5V (with 4.7K pull-up resistor to Signal )        
********************************************************************/
#define ONE_WIRE_BUS_PIN 22
OneWire oneWire(ONE_WIRE_BUS_PIN); // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass oneWire pointer to Dallas Temperature.

// Use DS18B20_find_address sketch to find these
DeviceAddress ProbeA = { 
  0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE 
}; 
DeviceAddress ProbeB = { 
  0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58 
};

/********************************************************************
* Global Variables
********************************************************************/
unsigned long sysTime;
float tempA = 0;  // stores results of reading the sensor
float tempB = 0;
float _tempA = 0; // stores the old result of tempA for updating LCD screen
float _tempB = 0;
char textA[8];    // buffer to store the results of dtostrf TODO: refactor so this isn't global
char textB[8];
char _textA[8];   // stores the old result of textA for updating LCD screen
char _textB[8];

/********************************************************************
* Set Up
********************************************************************/
void setup()
{
  Serial.begin(9600);
  Tft.init();  //init TFT library
  Tft.paintScreenBlack();  // Clear screen
  Tft.setDisplayDirect(UP2DOWN);
  Tft.drawString("A:",180,40,3,WHITE);
  Tft.drawString("B:",90,40,3,WHITE);
  Serial.print("Initializing Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);
  sensors.begin(); // Initialize the Temperature measurement library
  sensors.setResolution(ProbeA, 10); // set the resolution to 10 bit (Can be 9 to 12 bits; lower is faster)
  sensors.setResolution(ProbeB, 10);
  Serial.println();
  Serial.print("Number of devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());
  Serial.print("Getting temperatures...");  
  Serial.println("\n\r");
}

/********************************************************************
* Main Loop
********************************************************************/
void loop()
{
  delay(1000);
  sensors.requestTemperatures(); // Command all devices on bus to read temperature
  Serial.print("Probe A:\t");
  getTemperature(ProbeA, tempA);
  Serial.print("Probe B:\t");
  getTemperature(ProbeB, tempB);
  /*Serial.println(""); // DEBUG
  Serial.print("tempA: "); // DEBUG
  Serial.println(tempA); // DEBUG
  Serial.print("tempB: "); // DEBUG
  Serial.println(tempB); // DEBUG */
  Serial.println("\n\r");

  /* Convert floats to strings for LCD screen */
  dtostrf(tempA, 1, 2, textA); // Arguments are (float, width, precision, buffer)
  dtostrf(tempB, 1, 2, textB); // Arguments are (float, width, precision, buffer)
  /*Serial.print("textA: "); // DEBUG
  Serial.println(textA); // DEBUG
  Serial.print("textB: "); // DEBUG
  Serial.println(textB); // DEBUG
  Serial.println(""); // DEBUG */

  /* Display results on LCD screen only if they've changed */
  if (tempA != _tempA)
  {
    Tft.drawString(_textA,180,120,4,BLACK);
    Tft.drawString(textA,180,120,4,WHITE);
  }

  if (tempB != _tempB)
  {
    Tft.drawString(_textB,90,120,4,BLACK);
    Tft.drawString(textB,90,120,4,WHITE);
  }

  // Store current results for comparison
  _tempA = tempA;
  _tempB = tempB;
  strncpy(_textA, textA, 8);
  strncpy(_textB, textB, 8);
}

/*******************************************************************
* User Defined Functions
********************************************************************/
// This gets the temp, prints it to Serial, and assigns it to its
// global variable (for future printing to LCD screen)
void getTemperature(DeviceAddress deviceAddress, float& sensorName)
{

  float tempF = sensors.getTempF(deviceAddress);
  sensorName = tempF;

  if (tempF < -100.00) 
  {
    Serial.print("Error getting temperature");
  }
  else
  {
    Serial.print("F: ");
    Serial.print(tempF);
    Serial.print("\tTime: ");
    sysTime = millis();
    Serial.println(sysTime / 1000);
  }

  /* Sample Output
  Initializing Temperature Control Library Version 3.7.2
  Number of devices found on bus = 2
  Getting temperatures...

  Probe A:  F: 76.55  Time: 4
  Probe B:  F: 75.20  Time: 4
  */
}
