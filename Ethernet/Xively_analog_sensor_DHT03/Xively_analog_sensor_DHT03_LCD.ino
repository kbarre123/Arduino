#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>
#include <DHT.h>
#include <LiquidCrystal.h>

/********** SENSOR **********/
// Define analog pin for sensor;
int sensorPin = 4;

// Define the specific model of DHT03 per DHT.h
#define DHTTYPE DHT22

// Initialize DHT function
DHT dht(sensorPin, DHTTYPE);

/********** LED STATUS **********/
// Define LED pin
int ledPin = 5;

/********** LCD DISPLAY **********/
// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/********** ETHERNET **********/
// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your Xively key to let you upload data
char xivelyKey[] = "hwSs00KM9lHGuuAMn5FhkqPJtuACBrv88fyNz0XJc5L1pJVC";

// Xively feed ID
const long int xivelyFeed = 56125899;

// Define the strings for our datastream IDs
char tempId[] = "Temperature";
char humId[] = "Humidity";

// Define the strings for our datastream IDs
XivelyDatastream datastreams[] = {
    XivelyDatastream(tempId, strlen(tempId), DATASTREAM_FLOAT), 
    XivelyDatastream(humId, strlen(humId), DATASTREAM_FLOAT)
};

// Finally, wrap the datastreams into a feed; 1 is # of datastreams
XivelyFeed feed(xivelyFeed, datastreams, 2);
 
EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
    
    Serial.begin(9600);

    // Pin setup
    pinMode(sensorPin, INPUT);
    pinMode(ledPin, OUTPUT);

    // Wake up the LCD
    lcd.begin(16, 2);
    // Print status
    lcd.print("Connecting..."); 

    Serial.println("Starting single datastream upload to Xively...");
    Serial.println();

    // Connect to the internet
    while (Ethernet.begin(mac) != 1)
    {
        lcd.print("Trying again...");
        Serial.println("Error getting IP address via DHCP, trying again...");
        delay(10000);
    }
    
    // Wake up the sensor
    dht.begin();
}

void loop() {
    
    // Turn off status LED
    digitalWrite(ledPin, LOW);
    
    // Reading temperature or humidity takes about 250 ms
    // Sensor readings may also be up to 2 seconds old
    float h = dht.readHumidity();
    float c = dht.readTemperature();
    // Convert to Farenheit
    float t = (c * 9 / 5) + 32;
    // Pass the readings on to array to send to Xively
    datastreams[0].setFloat(t);
    datastreams[1].setFloat(h);
    
    // Print the readings to Serial
    Serial.print("Humidity: ");
    Serial.print(datastreams[0].getFloat());
    Serial.print("% \t");
    Serial.print("Temperature: ");
    Serial.print(datastreams[1].getFloat());
    Serial.println(" *F");

    // Send value to Xively
    Serial.println("Uploading it to Xively...");
    int ret = xivelyclient.put(feed, xivelyKey);
    
    // If uploaded to Xively, tuen on status LED
    if (ret) {
        digitalWrite(ledPin, HIGH);
    }
    else {
        digitalWrite(ledPin, LOW);
    }
    
    // Return message
    Serial.print("xivelyclient.put returned ");
    Serial.println(ret);

    Serial.println();
    delay(10000);
}

