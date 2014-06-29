/* TODO: 
    1) Implement LED temp/time scale (blue/green/yellow/red);.
    2) Implement function that beeps when target temp is achieved. Need a way to reset/mute beep.
    3) Implement second temp sensor to LCD
    4) Plan migration from breadboard to project board; plan mounting of LCD/Arduino/project board inside brew station.
*/

#include <OneWire.h>
#include <LiquidCrystal.h>
#include <DallasTemperature.h>

/********** SENSOR **********/
// Both data lines are in paralell
// Address A: 0x28, 0xE7, 0x30, 0x06, 0x06, 0x00, 0x00, 0xEE
// Address B: 0x28, 0xC0, 0x98, 0x05, 0x06, 0x00, 0x00, 0x58

int sensorPin = 11;
// Instantiate OneWire object
OneWire ds(sensorPin);

/********** LED **********/
// LED pin
int ledPin = 10;
// Turn on LED when <t> reaches this target temp(F)
int targetTemp = 82;
boolean ledState = false;

/********** LCD DISPLAY **********/
// Pin name on LCD:     { VSS, VDD, VO,       RS, RW,  E, D0, D1, D2, D3, D4, D5, D6, D7, A,   K   }
// Pin name on Arduino: { Grd, Vcc, 10k pot*, 4,  Gnd, 5, NA, NA, NA, NA, 6,  7,  8,  9,  Vcc, Grd }
// *10k pot uses ground-to-ground and wiper-to VO pin on LCD shield

// Initialize LCD Display object
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  // Pin setup
  //pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Print status
  lcd.clear();
  digitalWrite(ledPin, HIGH);
  lcd.print("* Bebop Robot *");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Let's Brew This!");
  delay(2000);
  lcd.clear();
}// End Setup

void loop() 
{
  // Reset LED and LCD display
  if (ledState == false)
  {
    digitalWrite(ledPin, LOW);
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }
  lcd.setCursor(0, 0);

  // Read sensor here.
  float readTemp = getTemp();

  // Validate reading and print to Serial
  if (isnan(readTemp)) 
  {
    Serial.println("Failed to read temp!");
  }
  else
  {
    float temp = (readTemp * 9.0 / 5.0) + 32.0;
    // Print temp to Serial
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *F");
    
    // Print temp to LCD
    lcd.print("Boil: ");
    lcd.print(temp);
    lcd.print(" ");
    
    // Check if targetTemp is achieved; turn on LED if so
    if (temp >= targetTemp) 
    {
      ledState = true;
      // TODO: Beep
    }
    else
    {
      ledState = false;
    }
  }
  delay(1000);
}// End Main Loop


// Returns the temp from DS18B20 in DEG Celsius
float getTemp()
{
  byte data[12];
  byte addr[8];
  
  // If no more sensors on the chain, reset search
  if (!ds.search(addr))
  {
    ds.reset_search();
    return -1000;
  }
  
  if (OneWire::crc8(addr, 7) != addr[7])
  {
    Serial.println("CRC is not valid!");
    return -1000;
  }
  
  if (addr[0] != 0x10 && addr[0] != 0x28)
  {
    Serial.println("Device is not recognized!");
    return -1000;
  }
  
  ds.reset();
  ds.select(addr);
  // Start conversion, w/ parasite power on at the end (IDK WFT this means)
  ds.write(0x44, 1);
  
  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad
  
  for (int i = 0; i < 9; i++) 
  {
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];
  
  float tempRead = ((MSB << 8) | LSB); // Using 2's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
}
