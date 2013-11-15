/* 
This sketch tests the functionality of two 8 channel 5VDC SSR's.
*/

// Define pins on Arduino that will control the relay.
#define CHANNEL_03 2
#define CHANNEL_04 3
#define CHANNEL_05 4
#define CHANNEL_06 5
#define CHANNEL_07 6
#define CHANNEL_08 7
#define CHANNEL_09 8
#define CHANNEL_10 9
#define CHANNEL_01 A4
#define CHANNEL_02 A5
#define CHANNEL_11 10
#define CHANNEL_12 11
#define CHANNEL_13 12
#define CHANNEL_14 13

// Define array of channels.
int channels[] = 
  {
    CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07, CHANNEL_08, CHANNEL_09, 
    CHANNEL_10, CHANNEL_01, CHANNEL_02, CHANNEL_11, CHANNEL_12, CHANNEL_13, CHANNEL_14
  };

// Define number of channels.
#define CHANNEL_COUNT 14

void setup()
{
  for(int i = 0; i < CHANNEL_COUNT; i++)
  { // Set up each channel as an output.
    pinMode(channels[i], OUTPUT);
  }
}

void loop()
{   
  testLights();
}

//***************** DEFINE FUNCTIONS *****************

// Power on self test.
void testLights()
{
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    analogWrite(channels[i], 255);
    delay(100);
    digitalWrite(channels[i], 0);
    delay(100);
  }
}

