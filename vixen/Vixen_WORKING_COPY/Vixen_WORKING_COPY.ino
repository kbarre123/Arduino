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
  
  pinMode(RANDOM_MODE_PIN, INPUT);

  powerOnSelfTest();
}

void loop()
{ // If switch is on, then turn lights on/off randomly
  if(analogRead(RANDOM_MODE_PIN > (1023 / 2)))
  {
    startingVixen = true;
    doRandomLights();
  }
  else
  { // Else, read data from Vixen
    if(startingVixen == true)
    {
      turnLightsOff();
      readFromVixen();
    }
  }
}

// Power on self test
void powerOnSelfTest()
{
  turnLightsOff();
  
  for(int channelIndex = 0; channelIndex < CHANNEL_COUNT; channelIndex++)
  {
    digitalWrite(channels[channelIndex], HIGH);
    delay(500);
    //digitalWrite(channels[channelIndex], LOW);
    //delay(500);
  }
  turnLightsOff();
}

// Turn lights off
void turnLightsOff()
{
  for(int channelIndex = 0; channelIndex < CHANNEL_COUNT; channelIndex++)
  {// Switch from 0 to 255 once I hook up the relays; they're polarity is opposite that of an LED
    digitalWrite(channels[channelIndex], LOW);
  }
}

// Do random light sequence
void doRandomLights()
{// Read value to pin A1 and generates a random sequence from it
  randomSeed(analogRead(A1));
  
  for(int channelIndex = 0; channelIndex < CHANNEL_COUNT; channelIndex++)
  {
    int randNumber = random(0, 255);
    randNumber = map(randNumber, 0, 255, 255, 0);
    if(randNumber <= 127)
    {
      digitalWrite(channels[channelIndex], HIGH);
    }
    else
    {
      digitalWrite(channels[channelIndex], LOW);
    }
  }
  
  delay(random(100, RANDOM_MODE_SPEED));
}

// Read data from Vixen
void readFromVixen()
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
