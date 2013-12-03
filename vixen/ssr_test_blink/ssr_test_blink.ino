/* 
This sketch tests the functionality of two 8 channel 5VDC SSR's.
*/

// Define pins on Arduino that will control the relay.
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
#define CHANNEL_13 A5
#define CHANNEL_14 A4
#define CHANNEL_15 A3
#define CHANNEL_16 A2

// Define array of channels.
int channels[] = 
  {
    CHANNEL_01, CHANNEL_02, CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07, 
    CHANNEL_08, CHANNEL_09, CHANNEL_10, CHANNEL_11, CHANNEL_12 , CHANNEL_13, CHANNEL_14, CHANNEL_15, CHANNEL_16
  }; 

// Define number of channels.
#define CHANNEL_COUNT 16

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
  digitalWrite(CHANNEL_01, HIGH);
  digitalWrite(CHANNEL_02, HIGH);
  digitalWrite(CHANNEL_03, HIGH);
  digitalWrite(CHANNEL_04, HIGH);
  digitalWrite(CHANNEL_05, HIGH);
  digitalWrite(CHANNEL_06, HIGH);
  digitalWrite(CHANNEL_07, HIGH);
  digitalWrite(CHANNEL_08, HIGH);
  digitalWrite(CHANNEL_09, HIGH);
  digitalWrite(CHANNEL_10, HIGH);
  digitalWrite(CHANNEL_11, HIGH);
  digitalWrite(CHANNEL_12, HIGH);
  digitalWrite(CHANNEL_13, HIGH);
  digitalWrite(CHANNEL_14, HIGH);
  digitalWrite(CHANNEL_15, HIGH);
  digitalWrite(CHANNEL_16, HIGH);
  delay(100);
  digitalWrite(CHANNEL_01, LOW);
  digitalWrite(CHANNEL_02, LOW);
  digitalWrite(CHANNEL_03, LOW);
  digitalWrite(CHANNEL_04, LOW);
  digitalWrite(CHANNEL_05, LOW);
  digitalWrite(CHANNEL_06, LOW);
  digitalWrite(CHANNEL_07, LOW);
  digitalWrite(CHANNEL_08, LOW);
  digitalWrite(CHANNEL_09, LOW);
  digitalWrite(CHANNEL_10, LOW);
  digitalWrite(CHANNEL_11, LOW);
  digitalWrite(CHANNEL_12, LOW);
  digitalWrite(CHANNEL_13, LOW);
  digitalWrite(CHANNEL_14, LOW);
  digitalWrite(CHANNEL_15, LOW);
  digitalWrite(CHANNEL_16, LOW);
  delay(200);
}

