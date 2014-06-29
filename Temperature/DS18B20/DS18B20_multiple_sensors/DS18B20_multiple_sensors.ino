/* YourDuino Multiple DS18B20 Temperature Sensors on 1 wire
 Connections:
 DS18B20 Pinout (Left to Right, pins down, flat side toward you)
 - Left   = Ground
 - Center = Signal (Pin 2):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
 - Right  = +5 or +3.3 V
 
 Questions: terry@yourduino.com 
 V1.01  01/17/2013 ...based on examples from Rik Kretzinger
 
/*-----( Import needed libraries )-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define ONE_WIRE_BUS_PIN 11

/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

/*-----( Declare Variables )-----*/
// See DS18B20_find_address sketch to find these

DeviceAddress ProbeA = { 
  0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE 
}; 
DeviceAddress ProbeB = { 
  0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58 
};

void setup()
{
  // start serial port to show results
  Serial.begin(9600);
  Serial.print("Initializing Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);

  // Initialize the Temperature measurement library
  sensors.begin();

  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(ProbeA, 10);
  sensors.setResolution(ProbeB, 10);
} // END SETUP

void loop()
{
  delay(1000);
  Serial.println();
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());   
  Serial.print("Getting temperatures... ");  
  Serial.println();   

  // Command all devices on bus to read temperature  
  sensors.requestTemperatures();  

  Serial.print("Probe A temperature is:   ");
  printTemperature(ProbeA);
  Serial.println();

  Serial.print("Probe B temperature is:   ");
  printTemperature(ProbeB);
  Serial.println();
} // END MAIN LOOP

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
} // END printTemperature() method
