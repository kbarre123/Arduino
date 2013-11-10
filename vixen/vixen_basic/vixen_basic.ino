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

// Define variable to hold # of channels
#define CHANNEL_COUNT 14

// Define array to hold all channels
int channels[] = 
{
  CHANNEL_01, CHANNEL_02, CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07,
  CHANNEL_08, CHANNEL_09, CHANNEL_10, CHANNEL_11, CHANNEL_12, CHANNEL_13, CHANNEL_14
};

// Define array to hold incoming data stream from Vixen
int incomingByte[14];

// Define baud rate. This figure must match that of your profile configuration in Vixen!
#define BAUD_RATE 57600

void setup()
{
  // Begin serial communication
  Serial.begin(BAUD_RATE);

  // Set up each channel as an output
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    pinMode(channels[i], OUTPUT);  
  }
}

void loop()
{
  // Read data from Vixen, store in array
  if (Serial.available() >= CHANNEL_COUNT)
  {
    for (int i=0; i<CHANNEL_COUNT; i++)
    {
      incomingByte[i] = Serial.read();
    }                                  
    // Write each byte of data from array to a pin on Arduino
    for (int i=0; i<CHANNEL_COUNT; i++)
    {
      digitalWrite(channels[i], incomingByte[i]);
    }
  }
}
