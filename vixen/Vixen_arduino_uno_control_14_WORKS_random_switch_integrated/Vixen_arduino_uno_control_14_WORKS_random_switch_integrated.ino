#define ledPin_1 A4
#define ledPin_2 A5
#define ledPin_3 2
#define ledPin_4 3
#define ledPin_5 4
#define ledPin_6 5
#define ledPin_7 6
#define ledPin_8 7
#define ledPin_9 8
#define ledPin_10 9
#define ledPin_11 10
#define ledPin_12 11
#define ledPin_13 12
#define ledPin_14 13

//#define i 0
int incomingByte[14];

#define RANDOM_MODE_PIN A0
#define RANDOM_MODE_SPEED 5000

int channels[] = 
  {
    ledPin_1, ledPin_2, ledPin_3, ledPin_4, ledPin_5, ledPin_6, ledPin_7,
    ledPin_8, ledPin_9, ledPin_10, ledPin_11, ledPin_12, ledPin_13, ledPin_14
  };
  
#define CHANNEL_COUNT 14
#define BAUD_RATE 57600

#define MODE_DIMMING 0
#define MODE_FULL 1
#define MODE MODE_FULL

boolean startingVixen = true;

void setup()
{
  Serial.begin(BAUD_RATE);

  for(int channelIndex = 0; channelIndex < CHANNEL_COUNT; channelIndex++)
  {
    pinMode(channels[channelIndex], OUTPUT);
  }
  
  //turnLightsOff();
  //powerOnSelfTest();
}

void loop()
{
  if (Serial.available() >= CHANNEL_COUNT)
  {
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      incomingByte[i] = Serial.read();
    }
    for (int i = 0; i < CHANNEL_COUNT; i++)
    {
      digitalWrite(channels[i], incomingByte[i]);
    }    
  }
}
