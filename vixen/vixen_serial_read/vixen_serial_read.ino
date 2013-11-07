/*********************************************************** 
 * This code controls an Arduino Uno via Vixen. I'm using two
 * Sainsmart 5VDC SSR's to control the 14 strands of lights.
 * Check out my blog at bogusbarrett.com for a detailed walk-
 * through of the build and setup of Vixen 3.
 *
 * This code is modified from that which was written by Scott Shaver.
 * Videos for Version 1 of the controller can be found at this URL:
 * http://www.youtube.com/watch?v=B5Wsovc2TSg&list=PLp21wF8e4XnjNSQHJcnvNSDdb1wsPOfHi 
 *
 ***********************************************************/

// which pins control which channels
#define CHANNEL01  A5
#define CHANNEL02  A4
#define CHANNEL03  2
#define CHANNEL04  3  // PWM
#define CHANNEL05  4
#define CHANNEL06  5  // PWM
#define CHANNEL07  6  // PWM
#define CHANNEL08  7
#define CHANNEL09  8
#define CHANNEL10  9  // PWM
#define CHANNEL11  10  // PWM
#define CHANNEL12  11  // PWM
#define CHANNEL13  12
#define CHANNEL14  13

// Which pins is the random/Vixen mode switch using
#define RANDOM_MODE_PININ A0
//#define RANDOM_MODE_PINOUT 53    //***************************************TODO: What is this?
#define RANDOM_MODE_SPEED 5000

// Array to store data for each channel
int channels[] = 
{
  CHANNEL01,CHANNEL02,CHANNEL03,CHANNEL04,CHANNEL05,CHANNEL06,CHANNEL07,CHANNEL08,CHANNEL09,
  CHANNEL10,CHANNEL11,CHANNEL12,CHANNEL13,CHANNEL14    
};

// How many channels will vixen be sending
#define CHANNEL_COUNT 14

// Speed for the com port for talking with vixen
#define VIXEN_COM_SPEED 57600

// Speed for talking with the serial monitor in the IDE
// #define PC_COM_SPEED 57600

// setup your choice of dimming values or just on/off values
#define MODE_DIMMING 0
#define MODE_FULL 1
#define MODE MODE_FULL

boolean startingVixen = true;

void setup()
{
  Serial.begin(VIXEN_COM_SPEED);
  //Serial1.begin(PC_COM_SPEED);    // ********************* I only have 1 serial port; this was written for a Mega. WTF???

  // set the channel pins to output mode
  for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++)
  {
    pinMode(channels[channelIndex],OUTPUT);
  }

  // set up the switch for Vixen or Random mode
  pinMode(RANDOM_MODE_PININ, INPUT);
  digitalWrite(RANDOM_MODE_PININ, HIGH); // turn on the internal pull-up resistor  ******************* What is this?
  //pinMode(RANDOM_MODE_PINOUT, OUTPUT);  // ******************* What is this?

  turnLightsOff();  
  powerOnSelfTest();
}

// !!!! note the PWM values that need to be sent to the relay board are reversed from the
// values coming in from Vixen.  Vixen 0-255 (off-on), Relays 255-0 (off-on)
void loop()
{
  if(analogRead(RANDOM_MODE_PININ) > (1023/2))  // Change the comparator sign to flip how the switch works
  { // blink at random mode
    startingVixen=true;
    doRandomLights();
  }
  else
  { // play from Vixen mode
    if(startingVixen==true)
      turnLightsOff();
    readFromVixen();
  }
}

void powerOnSelfTest()
{
  turnLightsOff();  
  Serial.println("Power on self test running.");
  for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++)
  {
    Serial.print("Channel: ");
    Serial.println(channelIndex+1, DEC);
    analogWrite(channels[channelIndex], 0); // turn on one channel at a time
    delay(500); // wait .5 seconds
    analogWrite(channels[channelIndex], 255);
  }
  turnLightsOff(); 
}

void turnLightsOff()
{
  //turn them all off
  for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++)
  {
    analogWrite(channels[channelIndex], 255);
  }
}

void doRandomLights()
{
  randomSeed(analogRead(1)); // Reads the volatage at this pin to seed the random # generator
  Serial.println("Writing random values.");
  for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++)
  {
    int randNumber = random(0, 255);
    randNumber = map(randNumber, 0, 255, 255, 0);
    if(randNumber<=127)
      digitalWrite(channels[channelIndex], 0);  // This was "analogWrite" for some reason
    else
      digitalWrite(channels[channelIndex], 255);  // This was "analogWrite" for some reason
    Serial.print(randNumber, DEC);
    Serial.print(",");
  }
  Serial.println("");
  delay(random(100, RANDOM_MODE_SPEED));
}

void outputToLights(unsigned char* buffer)
{
  for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++)
  {
    analogWrite(channels[channelIndex], buffer[channelIndex]);
    Serial.print(buffer[channelIndex], DEC);
    Serial.print(",");
  }
  Serial.println("");
}

void readFromVixen()
{
  Serial.println("Waiting for data from Vixen.");
  startingVixen = false;
  char *footer="VIXEN_END";
  unsigned char buffer[CHANNEL_COUNT];
  char buffer2[CHANNEL_COUNT];
  int index=0;
  unsigned long time = millis();

  //waitForVixenHeader();
  while (true) 
  {
    int inByte = Serial.read();  // This was "int inByte = Serial1.read(); However, I changed the Vixen serial designation before the setup loop.  May need to play with this.
    if(inByte==-1)
    {
      if(index==0 && millis()-time>1000) // we haven't read anything in a second
        return;
      continue;
    }
    time = millis();
    int lightByte = map(inByte, 0, 255, 255, 0);
    buffer[index] = lightByte;
    buffer2[index] = inByte;
    buffer[index+1] = 0;
    buffer2[index+1] = 0;
    index++;
    if(index==9 && strcmp(footer,buffer2)==0)
    {
      Serial.println(footer);
      return;
    }
    else if(index==CHANNEL_COUNT)
    {
      outputToLights(buffer);
      index=0;
    }
  }
  Serial.println("");
}

void waitForVixenHeader()
{
  char *header="VIXEN_START";
  char buffer[12];  // *********************Why is this 12 and not CHANNEL_COUNT?
  int index = 0;
  unsigned long time = millis();

  while (true) 
  {
    int inByte = Serial.read();  // This was "int inByte = Serial1.read();"
    if(inByte==-1)
    {
      if(index==0 && millis()-time>1000) // we haven't read anything in a second
        return;
      continue;
    }
    time = millis();
    buffer[index] = inByte;
    if(buffer[index]!=header[index])
    {// not the right sequence restart
      index=-1;
    }
    buffer[index+1] = 0; // add null
    index++;
    if(index==11 && strcmp(header,buffer)==0)
    {
      Serial.println(header);
      return;
    }
  }
}





