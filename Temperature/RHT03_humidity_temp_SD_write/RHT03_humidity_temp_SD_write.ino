// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include <DHT.h>
#include <SD.h>

#define DHTPIN 2     // what pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302)

const int chipSelect = 10; // ChipSelect pin for SD shield

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600); 
    Serial.print("Initializing SD card...");
    pinMode(10, OUTPUT); // Make sure CS pin for SD card is set to output.
    
    if (!SD.begin(chipSelect)) {
        Serial.println("card failed, or not present.");
        return;
    }
    Serial.println("card initialized.");

    dht.begin();  // Wake up the humidity/temp sensor
}

void loop() {
    // Make a string for assembling the data to log;
    String dataString = "";
    
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) {
        Serial.println("Failed to read from RHT03.");
    } 
    // Else, print to screen and SD card.
    else {
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
        
        // Open file with write permission.
        File dataFile = SD.open("datalog.txt", FILE_WRITE);
        
        // If the file is available, write to it...duh.
        if (dataFile) {
            dataFile.print(h);
            dataFile.print(" ");
            dataFile.println(t);
            dataFile.close();
        }
        // If the file isn't open, throw an error.
        else {
            Serial.println("Error opening datalog.txt");
        }
        
        delay(1000);
    }
}

