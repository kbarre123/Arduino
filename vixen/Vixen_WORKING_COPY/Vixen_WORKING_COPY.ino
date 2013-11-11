// This should be on the switch branch only.

/* 
This sketch allows the Arduino to read 14 bytes of data from Vixen and turn on
its pins accordingly, which in turn control a solid state relay hooked up to Xmas lights.
*/

// Define pins on Arduino that will control the relay.
#define CHANNEL_01 A4
#define CHANNEL_02 A5
#define CHANNEL_03 2
#define CHANNEL_04 3
#define CHANNEL_05 4
#define CHANNEL_06 5
#define CHANNEL_07 6
#define CHANNEL_08 7
#define CHANNEL_09 8
#define CHANNEL_10 9
#define CHANNEL_11 10
#define CHANNEL_12 11
#define CHANNEL_13 12
#define CHANNEL_14 13

// Define array of channels.
int channels[] = 
  {
    CHANNEL_01, CHANNEL_02, CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07,
    CHANNEL_08, CHANNEL_09, CHANNEL_10, CHANNEL_11, CHANNEL_12, CHANNEL_13, CHANNEL_14
  };

// Define number of channels.
#define CHANNEL_COUNT 14

// Define buffer to store data from Vixen until it gets written to the Arduino.
int incomingByte[14];

// Define the pin that will trigger "random mode".
#define RANDOM_MODE_PIN A0
#define RANDOM_MODE_SPEED 5000

// Define the baud rate for communication with Vixen. This must match that of the Vixen profile!!  
#define BAUD_RATE 57600

void setup()
{
  Serial.begin(BAUD_RATE);

  for(int i = 0; i < CHANNEL_COUNT; i++)
  { // Set up each channel as an output.
    pinMode(channels[i], OUTPUT);
  }
  // Set up the pin for random mode as input.
  pinMode(RANDOM_MODE_PIN, INPUT);

  powerOnSelfTest();
}

void loop()
{ // If switch is on, then turn lights on/off randomly.
  if(analogRead(RANDOM_MODE_PIN > (1023 / 2)))
  {
    doRandomLights();
  }
  else
  { // Else, read data from Vixen.
    turnLightsOff();
    readFromVixen();
  }
}

// Turn lights off
void turnLightsOff()
{
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {// Switch from 0 to 255 once I hook up the relays; they're polarity is opposite than LEDs'.
    digitalWrite(channels[i], LOW);
  }
}

// Power on self test.
void powerOnSelfTest()
{
  turnLightsOff();
  
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    digitalWrite(channels[i], HIGH);
    delay(500);
    digitalWrite(channels[i], LOW);
  }
  turnLightsOff();
}

// Do random light sequence.
void doRandomLights()
{// Read value to pin A1 and generates a random sequence from it.
  randomSeed(analogRead(A1));
  
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    int randNumber = random(0, 255);
    randNumber = map(randNumber, 0, 255, 255, 0);
    if(randNumber <= 127)
    {
      digitalWrite(channels[i], HIGH);
    }
    else
    {
      digitalWrite(channels[i], LOW);
    }
  }
  
  delay(random(100, RANDOM_MODE_SPEED));
}

// Read data from Vixen.
void readFromVixen()
{
  if (Serial.available() >= CHANNEL_COUNT)
  {  // Store incoming bytes to buffer.
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      incomingByte[i] = Serial.read();
    }
    // Write buffer to channels.
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      digitalWrite(channels[i], incomingByte[i]);
    }    
  }
}
