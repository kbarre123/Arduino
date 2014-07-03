/* TODO:
 * Implement LED temp/time scale (blue/green/yellow/red);.
 * Plan migration from breadboard to project board; plan mounting of LCD/Arduino/project board inside brew station.
 */

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
// connected b/t Signal and +5 as noted above. Ground and +5 pins both hooked up to
// Ground line (nothing on +5 line, other than resistor connected to Signal line

// See DS18B20_find_address sketch to find these:
// Sensor A: 0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE
// Sensor B: 0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58

// Define data pin for OneWire bus (all sensors parallel on this pin)
#define ONE_WIRE_BUS_PIN 12
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

/********** RGB LED **********/
// See LED_RGB sketch for pinout and resistor documentation
int redPin = 11;
int greenPin = 10;
int bluePin = 9;
// Turn on LED when boil temp reaches target(F)
int boilTarget = 85;
// Declare global variables for use in loop() and indicate() methods
float tempBoil;
float tempMash;

/********** LCD DISPLAY **********/
// Pin name on LCD:     { VSS, VDD, VO,       RS, RW,  E, D0, D1, D2, D3, D4, D5, D6, D7, A,   K   }
// Pin name on Arduino: { Gnd, Vcc, 50k pot*, 2,  Gnd, 3, NA, NA, NA, NA, 4,  5,  6,  7,  Vcc, Grd }
// *50k pot uses ground-to-ground and wiper-to VO pin on LCD shield

// Initialize LCD Display object
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//**************** BUZZER ****************/
// Hook Grd on buzzer to Grd, other to pin; no resistor used
int buzzerPin = 8;
int notes[] = {  // Notes in the melody:
  NOTE_C8
};

int notesSize = sizeof(notes) / sizeof(int);

int notesLength[] = {  // Note durations: 4 = quarter note, 8 = eighth note, etc.:
  4
};

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

  // Pin setup
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Print boot message
  lcd.clear();
  setColor(255, 0, 0);
  lcd.print("* Bebop Robot *");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Let's Brew This!");
  delay(2000);
  lcd.clear();

  Serial.print("Initializing Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());

}// End setup()

void loop() 
{
  // Reset LCD display
  lcd.setCursor(0, 0);

  Serial.println("Getting temperatures... ");   

  // Command all devices on bus to read temperature  
  sensors.requestTemperatures();  

  // Read temps
  tempBoil = readTemp(SensorA);
  tempMash = readTemp(SensorB);

  // Print temps to Serial
  Serial.print("Boil Temp: ");
  Serial.print(tempBoil);
  Serial.println(" *F");

  Serial.print("Mash Temp: ");
  Serial.print(tempMash);
  Serial.println(" *F");

  // Print temps to LCD
  lcd.print("Boil: ");
  lcd.print(tempBoil);
  lcd.print(" *F");
  lcd.setCursor(0, 1);

  lcd.print("Mash: ");
  lcd.print(tempMash);
  lcd.print(" *F");
  
  // Test temp and indicate accordingly
  indicate(tempBoil);
  
  delay(1000);
}// End loop()




/********** USER DEFINED METHODS **********/

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
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
} // END

/** 
 * Test the temperature and indicate accordingly.
 * @param temperature.
 */
void indicate(float temp)
{
  temp = tempBoil;
  
  if (tempBoil <= 84)
  {
    setColor(0, 0, 255);
  }
  else if ( (84 < tempBoil) && (tempBoil <= 85) )
  {
    setColor(0, 255, 0);  
  }
  else
  {
    setColor(255, 0, 0);
    for (int i = 0; i < notesSize ; i++) 
    {
      int notesDuration = 1000/notesLength[i];
      tone(buzzerPin, notes[i], notesDuration);
    }
  }
}// END
