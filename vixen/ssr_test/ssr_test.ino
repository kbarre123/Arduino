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
<<<<<<< HEAD

=======
>>>>>>> 425792b7ac42ddb4ae9a89242ccb773b4375be24

// Define array of channels.
int channels[] = 
  {
<<<<<<< HEAD
    CHANNEL_01, CHANNEL_02, CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07, CHANNEL_08,
    CHANNEL_09, CHANNEL_10, CHANNEL_11, CHANNEL_12, CHANNEL_13, CHANNEL_14, CHANNEL_15, CHANNEL_16 
  };
=======
    CHANNEL_01, CHANNEL_02, CHANNEL_03, CHANNEL_04, CHANNEL_05, CHANNEL_06, CHANNEL_07, 
    CHANNEL_08, CHANNEL_09, CHANNEL_10, CHANNEL_11, CHANNEL_12, CHANNEL_13, CHANNEL_14, CHANNEL_15, CHANNEL_16 
  }; // 
>>>>>>> 425792b7ac42ddb4ae9a89242ccb773b4375be24

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
  for(int i = 0; i < CHANNEL_COUNT; i++)
  {
    digitalWrite(channels[i], HIGH);
<<<<<<< HEAD
    delay(100);
  }
  for(int i = CHANNEL_COUNT; i >= 0; i--)
  {
    digitalWrite(channels[i], LOW);
    delay(100);
=======
    delay(1000);
    //digitalWrite(channels[i], LOW);
    //delay(100);
>>>>>>> 425792b7ac42ddb4ae9a89242ccb773b4375be24
  }

}

