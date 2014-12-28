// Constants
const int numLEDs = 6;
const int numReadings = 20;
const int readingDelay = 50;

// Analog-in pins
int sensorPin = A2;

int readings[numReadings]; // Array to hold readings from analog-in
int index = 0; // Index of current reading
int total = 0; // Running total or readings
int average = 0;  // Average of readings

void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < numReadings; i++)  // Initialize all readings to 0
        readings[i] = 0;
}

void loop()
{
    total -= readings[index];  // Subtract the last reading
    readings[index] = analogRead(sensorPin);  // Read the sensor
    total += readings[index];  // Add the new reading to the total
    index++;  // Advance to the next position in the array
    
    if (index >= numReadings) // If at end of array, go back to beginning
        index = 0;
        
    average = total / numReadings;
    Serial.println(average);
    delay(readingDelay);
}
