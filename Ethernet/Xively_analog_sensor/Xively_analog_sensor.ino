#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your Xively key to let you upload data
char xivelyKey[] = "noAhpE3C9GmpXlaD7rsWcI66zl4XVZxXFzLEcbpG2b4WwnSZ";

// Xively feed ID
const long int xivelyFeed = 709320001;

// Analog pin which we're monitoring (0 and 1 are used by the Ethernet shield)
int sensorPin = 0;

// Define the strings for our datastream IDs
char sensorId[] = "Temperature";

// Define the strings for our datastream IDs
XivelyDatastream datastreams[] = {
    XivelyDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT)
};

// Finally, wrap the datastreams into a feed; 1 is # of datastreams
XivelyFeed feed(xivelyFeed, datastreams, 1);
 
EthernetClient client;
XivelyClient xivelyclient(client);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    // Pin setup
    pinMode(sensorPin, INPUT);

    Serial.println("Starting single datastream upload to Xively...");
    Serial.println();

    while (Ethernet.begin(mac) != 1)
    {
        Serial.println("Error getting IP address via DHCP, trying again...");
        delay(15000);
    }
}

void loop() {
    
    // Read sensor value
    int sensorValue = analogRead(sensorPin);
    datastreams[0].setFloat(sensorValue);
    
    // Print the sensor value
    Serial.print("Read sensor value ");
    Serial.println(datastreams[0].getFloat());

    // Send value to Xively
    Serial.println("Uploading it to Xively...");
    int ret = xivelyclient.put(feed, xivelyKey);
    
    // Return message
    Serial.print("xivelyclient.put returned ");
    Serial.println(ret);

    Serial.println();
    delay(15000);
}

