/*TODO: 20131031 - LED2 was used in the original sketch for testing purposes of the LCD backlight.
Find out which pin the LCD backlight switch is and update the backlightFlash() function.  Set up
the LCD screen w/ resistors and make sure everything goes to the Arduino right.
*/

// Ethernet Shield uses pins 2, 3, 10-13

#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// Declare sensor and LED pins
int sensorPin = 4;
int ledPin = 5;

// LCD STUFF************************************
const int TxPin = 1;
const int lineDelay = 500; // Time b/t first line sent to LC and second line
const int clearDelay = 2000; // Time after a message is written before it is cleared
const int backLight = A5; // LCD backlight
int ledState = LOW; // Set LED low
long previousMillis = 0; // Will store last time LED was updated
long interval = 1000;

// Variables for LED fade
int LEDFadeValue = 0;
unsigned long fadeTime = 0;
int x = 0;
int y = 0;

// Variables for Backlight Flash
unsigned long bFTime = 0;
int bF = 1;
int flashCounter = 0;
int flashCount = 11; // Number of times backlight flashed b/t messages
int nextMesDelay = 300; // Time after clear a message before flash the backlisht
int messageVar = 0;

// Variables for LCD message write
int displayCount = 1;

// MESSAGES TO DISPLAY ON LCD
char* displayText[][2]={
    {"TEMP/HUMIDITY"},
    {"EAST FELICIANA"},
    {"JACKSON"},
    {"LOUISIANA"}
};

unsigned long LCDTime = 0;
int NumMessages = 0;

SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

// LCD STUFF END *******************************

// Define the specific model of DHT03 per DHT.h
#define DHTTYPE DHT22

// Initialize DHT function
DHT dht(sensorPin, DHTTYPE);

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
    pinMode(TxPin, OUTPUT); // LCD stuff????
    digitalWrite(TxPin, HIGH); // LCD stuff???
    
    mySerial.write(12); // Clear
    mySerial.write(17); // Turn backlight on
    mySerial.write(22); // Turn backlight on
    delay(5); // Required delay
    
    // Print to LCD
    mySerial.print(" BOOTING UP... "); 
    delay(3000); // Wait 3 seconds
    mySerial.write(13); // Form feed 
    mySerial.print(" Version 1.0 "); // Second line // 
    delay(3000); 
    mySerial.write(12); // Clear
    fadeTime = millis(); 
    flashCounter = 0; 
    displayCount = 1; 
    NumMessages = sizeof(displayText)/sizeof(displayText[0]); 

    // Print to serial
    Serial.println("Starting single datastream upload to Xively...");
    Serial.println();

    while (Ethernet.begin(mac) != 1)
    {
        Serial.println("Error getting IP address via DHCP, trying again...");
        delay(10000);
    }
    
    // Wake up the sensor
    dht.begin();
}

void loop() {
    
    ledFade(); if(flashCounter == flashCount) LCDMessage(); // If we've finished Flashing, send the next message to the LCD backlightFlash();
    
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
        delay(1000);
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

void LCDMessage() { 

	unsigned long currenTime = millis();

	switch (displayCount) { 

		case 1: mySerial.write(17); messageVar = random(0, NumMessages); // Select a random string from the array
		mySerial.print(displayText[messageVar][0]); // Print First line to LCD 
		LCDTime = millis(); 
		displayCount = 2; 
		break; 

		case 2: if(currenTime > (LCDTime + lineDelay)) {  // Delay for lineDelay sec before displaying next line, not sure why.  
			mySerial.write(13); // Carriage Return 
			mySerial.print(displayText[messageVar][1]); 
			LCDTime = millis(); 
			displayCount = 3; 
		}
		break; 

		case 3: if(currenTime > (LCDTime + clearDelay)) { 
			mySerial.write(12); 
			mySerial.write(18); 
			displayCount = 4; 
			LCDTime = millis(); 
		} 

		case 4: if(currenTime > (LCDTime + nextMesDelay)) { 
			flashCounter = 0; 
			flashCount = random(3,20); // Set the number of Backlight flashes to a random number of max value 20 
			displayCount = 1; 
		} 
		break; 
		default: displayCount = 1; 
		break; 
	}
}

/* Function for flasing the LCD backlight on and off */ \
void backlightFlash() { 
	unsigned long currenTime = millis();

	if(flashCounter<flashCount) { 
		switch (bF) { 
			case 1: mySerial.write("17");
			digitalWrite(LED2, HIGH); // Used for Testing 
			bF = 2; 
			bFTime = millis(); 
			break; 

			case 2: if(currenTime > (bFTime + 100)) bF=3; 
			break; 

			case 3: mySerial.write("18"); //
			digitalWrite(LED2, LOW); // Used for Testing 
			bF = 4; 
			bFTime = millis(); 

			case 4: if(currenTime > (bFTime + 100)) { 
				bF=1; flashCounter++; 
			} 
			break; 
			default: bF = 1; 
			break; 
		} 
	}

}

/** Function for Fading the LED on and off without delaying the code **/ 
void ledFade() {

    unsigned long currenTime = millis();

    if(currenTime > (fadeTime + 5)) {
        analogWrite(ledPin, x); 
        if(y) {x++;} if(!y) {
            x--;
        } 
        if(x>250) {
            y=0;
        } 
        if(x<10) {
            y=1;
        } 
        fadeTime = millis(); 
    } 
}
