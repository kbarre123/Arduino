// Brew Test Branch

/* TODO: 
    1) Implement LED blink when target temp is achieved;
    2) Implement piezzo beep when target temp is achieved;
*/

#include <DHT.h>
#include <SPI.h>
#include <LiquidCrystal.h>

/********** SENSOR **********/
// RHT03 sensor pin.
int sensorPin = 13;

// Define the specific model of RHT03 per DHT.h.
#define DHTTYPE DHT22

// Initialize DHT object.
DHT dht(sensorPin, DHTTYPE);

/********** LED STATUS **********/
// LED pin.
int ledPin = 12;

/********** LCD DISPLAY **********/
// Pin name on LCD:     { VSS, VDD, VO,       RS, RW,  E, D0, D1, D2, D3, D4, D5, D6, D7, A,   K   }
// Pin name on Arduino: { Grd, Vcc, 10k pot*, 4,  Gnd, 5, NA, NA, NA, NA, 6,  7,  8,  9,  Vcc, Grd }
// *10k pot uses ground-to-ground and wiper-to VO pin on LCD shield

// Initialize LCD Display object.
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  // Pin setup
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Print status
  lcd.clear();
  lcd.print("* Bebop Robot *");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Let's Brew This!");
  delay(2000);
  lcd.clear();

  // Wake up the sensor
  Serial.println("Waking up sensor...");
  dht.begin();
}

void loop() 
{
  // Reset LED and LCD display
  digitalWrite(ledPin, LOW);
  lcd.setCursor(0, 0);

  // Read sensor, may take about 250 ms
  float h = dht.readHumidity();
  float c = dht.readTemperature();
  // Convert to Farenheit
  float t = (c * 9 / 5) + 32;

  // Validate readings and print to Serial
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("*F \t");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println("%");

    // Print the readings to LCD
    lcd.print("T: ");
    lcd.print(t);
    lcd.print(" *F");
    lcd.setCursor(0, 1);
    lcd.print("H: ");
    lcd.print(h);
    lcd.print(" % ");
  }
  
  delay(1000);
}


