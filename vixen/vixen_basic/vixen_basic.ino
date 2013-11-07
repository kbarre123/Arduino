// Define output pins
int Chan1 = 0;  
int Chan2 = 1;  
int Chan3 = 2;  
int Chan4 = 3;  
int Chan5 = 4;
int Chan6 = 5;
int Chan7 = 6;
int Chan8 = 7;
int Chan9 = 8;
int Chan10 = 9;
int Chan11 = 10;
int Chan12 = 11;
int Chan13 = 12;
int Chan14 = 13;

int i = 0;     // Loop counter

#define CHANNEL_COUNT 14

// Array to hold data
int channels[] = 
{
  Chan1, Chan2, Chan3, Chan4, Chan5, Chan6, Chan7, Chan8, Chan9, Chan10, 
  Chan11, Chan12, Chan13, Chan14
};

int buffer[14];   // Array to store the values from the serial port

// Setup all the things
void setup()
{
  Serial.begin(57600);      // set up Serial at 57,600 baud

  // Set pins as outputs
  for(int channelIndex = 0; channelIndex < CHANNEL_COUNT; channelIndex++)
  {
    pinMode(channels[channelIndex], OUTPUT);
  }
}

// Loop all the things
void loop()
{  // Read serial data from Vixen
  if (Serial.available() >= CHANNEL_COUNT) 
  { // read the oldest byte in the serial buffer:
    for (int i = 0; i < CHANNEL_COUNT; i++) 
    { // read each byte
      buffer[i] = Serial.read();
    } // write each byte
    for(int i = 0; i < CHANNEL_COUNT; i++)
    {
      analogWrite(channels[i], buffer[0]);
    }
  }
}

