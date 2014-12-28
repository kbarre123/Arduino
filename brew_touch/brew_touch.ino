/********************************************************************
 * Read and display data from two temp sensors on a touchscreen.
 *
 * Seeedstudio LCD 2.8" TFT touchscreen
 * 2 X DS18B20 waterproof temp sensors
 * Arduino Mega
 *******************************************************************/

 /******************************************************************
 * Connections for sensors:
 * DS18B20 Pinout (Left to Right, pins down, flat side toward you)
 * - Left   = Ground
 * - Center = Signal (Pin 2):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
 * - Right  = +5 or +3.3 V
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
********************************************************************/
#define ONE_WIRE_BUS_PIN 11
OneWire oneWire(ONE_WIRE_BUS_PIN); // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass oneWire pointer to Dallas Temperature.

// Use DS18B20_find_address sketch to find these
DeviceAddress ProbeA = { 
  0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE 
}; 
DeviceAddress ProbeB = { 
  0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58 
};

float tempUpper = 0;
float tempMash = 0;
int DS18S20_Pin = 22; //DS18S20 Signal pin
OneWire ds(DS18S20_Pin); //Temperature chip i/o

void setup()
{
  Serial.begin(9600);
  Tft.init();  //init TFT library
  Tft.paintScreenBlack();  // Clear screen
  Tft.setDisplayDirect(UP2DOWN);
  Tft.drawString("Upper: ",220,20,2,WHITE);
  Tft.drawString("Mash: ",180,20,2,WHITE);
  Serial.print("Initializing Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);
  sensors.begin(); // Initialize the Temperature measurement library
  sensors.setResolution(ProbeA, 10); // set the resolution to 10 bit (Can be 9 to 12 bits; lower is faster)
  sensors.setResolution(ProbeB, 10);
  Serial.println();
  Serial.print("Number of devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());
  Serial.print("Getting temperatures...");  
  Serial.println();   
}

void loop()
{
  delay(1000);
  sensors.requestTemperatures(); // Command all devices on bus to read temperature
  Serial.print("Probe A:  ");
  printTemperature(ProbeA);
  Serial.println();

  Serial.print("Probe B:  ");
  printTemperature(ProbeB);
  Serial.println();

  // OLD CODE HERE; DON'T DELETE YET!!!
  /* Convert sensor data of Upper Pot to string for display. 
  char textUpper[8]; // buffer to store the results of dtostrf
  dtostrf(tempUpper, 1, 2, textUpper);  // Arguments are (float, width, precision, buffer)
  // Convert sensor data of Mash Tun to string for display
  char textMash[8];  // buffer to store the results of dtostrf
  dtostrf(tempMash, 1, 2, textMash);
  // Display results
  Tft.drawString(textUpper,220,180,2,WHITE);
  //Serial.print("textUpper: ");  // DEBUG
  //Serial.println(textUpper);  // DEBUG
  Tft.drawString(textMash,180,180,2,WHITE);
  //Serial.print("textMash: ");  // DEBUG
  //Serial.println(textMash);  // DEBUG
  //Serial.println("");  // DEBUG
  */
}

/*******************************************************************
* USER DEFINED FUNCTIONS
********************************************************************/

void printTemperature(DeviceAddress deviceAddress)
{

  float tempC = sensors.getTempC(deviceAddress);

  if (tempC == -127.00) 
  {
    Serial.print("Error getting temperature  ");
  } 
  else
  {
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }

  /* Sample Output
  * TODO: copy sample output once finalized
  */
}
