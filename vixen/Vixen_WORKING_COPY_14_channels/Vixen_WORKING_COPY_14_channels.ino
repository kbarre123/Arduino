/* 
This sketch allows the Arduino to read 14 bytes of data from Vixen and turn on
its pins accordingly, which for testing purposes are represented by 14 LEDs.
*/

// Define pins on Arduino that will control the LEDs.
#define CHANNEL_01 2
#define CHANNEL_02 3
#define CHANNEL_03 4
#define CHANNEL_04 5
#define CHANNEL_05 6
#define CHANNEL_06 7
#define CHANNEL_07 8
#define CHANNEL_08 9
#define CHANNEL_09 10
#define CHANNEL_10 11
#define CHANNEL_11 12
#define CHANNEL_12 13
#define CHANNEL_13 A4
#define CHANNEL_14 A5

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
#define RANDOM_MODE_SPEED 1000

// Define the baud rate for communication with Vixen. This must match that of the Vixen profile!!  
#define BAUD_RATE 38400

/*
On first pass through the main loop whilst not in random-mode, the program is forced to turn off
the lights, flush the serial buffer, and then proceed to readFromVixen. Once the program
enters readFromVixen(), the startingVixen variable gets flipped to false, allowing the program to
progress through the 'else' portion of the main loop, which doesn't include flushing the buffer and
turning the lights off; it will only 'readFromVixen()'.
*/
boolean startingVixen = true;

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
  if(analogRead(RANDOM_MODE_PIN) > (1023 / 2))
  {
    startingVixen = true;
    doRandomLights();
  }
  else
  { // Else, read data from Vixen. If first time in loop, turn off lights and read. Else, just read.
    if(startingVixen == true)
    {
      turnLightsOff();
      Serial.flush();
      readFromVixen();
    }
    else
    {
      readFromVixen();  
    }
  }
}

//***************** DEFINE FUNCTIONS ************************

// Turn lights off
void turnLightsOff()
{
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {// Switch from 0 to 255 once I hook up the relays; they're polarity is opposite than LEDs'.
    analogWrite(channels[i], 0);
  }
}

// Power on self test.
void powerOnSelfTest()
{
  turnLightsOff();
  
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    analogWrite(channels[i], 255);
    delay(500);
    analogWrite(channels[i], 0);
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
      analogWrite(channels[i], 255);
    }
    else
    {
      analogWrite(channels[i], 0);
    }
  } 
  delay(random(1000, RANDOM_MODE_SPEED));
}

// Read data from Vixen.
void readFromVixen()
{
  startingVixen = false;
  if (Serial.available() >= CHANNEL_COUNT)
  {  // Store incoming bytes to buffer.
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      incomingByte[i] = Serial.read();
    }
    // Write buffer to channels.
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      analogWrite(channels[i], incomingByte[i]);
    }    
  }
}
