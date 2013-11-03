#include <SPI.h>
#include <Ethernet.h> //Uses pins 2, 3, 10-13 for connection
#include <HttpClient.h>
#include <Xively.h>
#include <DHT.h>
#include <LiquidCrystal.h>

/********** SENSOR **********/
// Analog pin which we're monitoring;
int sensorPin = 4;

// Define the specific model of DHT03 per DHT.h
#define DHTTYPE DHT22

// Initialize DHT object
DHT dht(sensorPin, DHTTYPE);

/********** LED STATUS **********/
// LED status
int ledPin = 0;

/********** LCD DISPLAY **********/
// LCD Display object
LiquidCrystal lcd(A0, 5, 6, 7, 8, 9); //TODO: NEED TO CHANGE THESE PINS; CONFLICT WITH ETHERNET

// Pins {VSS, VDD, VO,      RS, RW,  E, D0, D1, D2, D3, D4, D5, D6, D7, A,   K}
// Pins {Grd, Vcc, 10k pot,  4, Gnd, 5, NA, NA, NA, NA,  6,  7,  8,  9, Vcc, Grd}
// 10k pot uses ground-to-ground and wiper-to VO pin on LCD shield

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
    lcd.begin(16, 2);
    
    // Pin setup
    pinMode(sensorPin, INPUT);
    pinMode(ledPin, OUTPUT);
    
    // Print status
    lcd.clear();
    lcd.print("Connecting...");
    Serial.println("Starting datastream upload to Xively...");
    Serial.println();

    while (Ethernet.begin(mac) != 1)
    {
        // Print connection error message
        lcd.print("Connection error");
        lcd.setCursor(0, 1);
        lcd.print("Retrying...");
        Serial.println("Error getting IP address via DHCP, trying again...");
        delay(10000);
    }
    
    // Wake up the sensor
    dht.begin();
}

void loop() {
    
    // Reset status LED and LCD display
    digitalWrite(ledPin, LOW);
    lcd.clear();
    
    // Read sensor, may take about 250 ms
    float h = dht.readHumidity();
    float c = dht.readTemperature();
    // Convert to Farenheit
    float t = (c * 9 / 5) + 32;
    // Pass the readings on to array to print and send to Xively
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
    
    // If uploaded to Xively, turn on status LED and print to LCD
    if (!ret) {
        digitalWrite(ledPin, LOW);
        lcd.print("Upload error!");
    }
    else {
        digitalWrite(ledPin, HIGH);
        lcd.print("Data uploaded!");
        delay(1000);
    }
    
    // Print the readings to LCD
    lcd.clear();
    lcd.print("H: ");
    //int hum = round(datastreams[1].getFloat());
    lcd.print(datastreams[1].getFloat());
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("T: ");
    //int temp = round(datastreams[0].getFloat());
    lcd.print(datastreams[0].getFloat());
    lcd.print("*F");
    
    // Print return message to Serial
    Serial.print("xivelyclient.put returned ");
    Serial.println(ret);

    Serial.println();
    delay(10000);
}

