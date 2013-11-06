// which pins control which channels
#define CHANNEL01  2
#define CHANNEL02  3
#define CHANNEL03  4
#define CHANNEL04  5
#define CHANNEL05  6
#define CHANNEL06  7
#define CHANNEL07  8
#define CHANNEL08  9
#define CHANNEL09  10
#define CHANNEL10  11
#define CHANNEL11  12
#define CHANNEL12  13

// Which pins is the random/Vixen mode switch using
#define RANDOM_MODE_PININ 0
#define RANDOM_MODE_PINOUT 53    //***************************************TODO: What is this?
#define RANDOM_MODE_SPEED 5000

int channels[] = 
{
    CHANNEL01,CHANNEL02,CHANNEL03,CHANNEL04,CHANNEL05,CHANNEL06,CHANNEL07,CHANNEL08,CHANNEL09,
    CHANNEL10,CHANNEL11,CHANNEL12
};

// how many channel will vixen be sending
#define CHANNEL_COUNT 12

// speed for the com port for talking with vixen
#define VIXEN_COM_SPEED 57600

// speed for talking with the serial monitor in the IDE
#define PC_COM_SPEED 57600

// setup your choice of dimming values or just on/off values
// the relays don't seem to be able to dim the lights so it looks
// like I will have to build dimmer circuits for next year. This
// doesn't change, just have to remove the relay bord and replace
// it with a dimmer circuit for each relay.
#define MODE_DIMMING 0
#define MODE_FULL 1
#define MODE MODE_FULL

boolean startingVixen = true;

void setup()
{
    Serial.begin(PC_COM_SPEED);
    //Serial.begin(VIXEN_COM_SPEED);    // ********************* This was "Serial1.begin(VIXEN_COM_SPEED); I only have 1 serial port (this was written for a Mega. WTF???

    // set the channel pins to output mode
    for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++){
        pinMode(channels[channelIndex],OUTPUT);
    }

    // set up the switch for Vixen or Random mode
    pinMode(RANDOM_MODE_PININ, INPUT);
    digitalWrite(RANDOM_MODE_PININ, HIGH); // turn on the internal pull-up resistor  ******************* What is this?
    pinMode(RANDOM_MODE_PINOUT, OUTPUT);

    turnLightsOff();  
    powerOnSelfTest();
}

// !!!! note the PWM values that need to be sent to the relay board are reversed from the
// values coming in from Vixen.  Vixen 0-255 (off-on), Relays 255-0 (off-on)
void loop()
{
    if(digitalRead(RANDOM_MODE_PININ)==LOW){ // blink at random mode
        startingVixen=true;
        doRandomLights();
    }
    else{ // play from Vixen mode
        if(startingVixen==true)
            turnLightsOff();
        readFromVixen();
    }
}

void powerOnSelfTest()
{
    Serial.println("Power on self test running.");
    for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++){
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
    for(int channelIndex=0;channelIndex<16;channelIndex++){
        analogWrite(channels[channelIndex], 255);
    }
}

void doRandomLights()
{
    randomSeed(analogRead(0));
    Serial.println("Writing random values.");
    for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++){
        if(MODE == MODE_DIMMING)
        {
            int randNumber = random(255);
            //randNumber = map(randNumber, 0, 255, 255, 0); // This is mapped in the reverse direction b/c apparently Vixen operates opposite from how you'd think it does.
            analogWrite(channels[channelIndex], randNumber);
            Serial.print(randNumber, DEC);
            Serial.print(",");
        }
        else // not dimming, just on or off
        {
            int randNumber = random(0, 255);
            //randNumber = map(randNumber, 0, 255, 255, 0);
            if(randNumber<=128)
                analogWrite(channels[channelIndex], 0);
            else
                analogWrite(channels[channelIndex], 255);
            Serial.print(randNumber, DEC);
            Serial.print(",");
        }
    }
    Serial.println("");
    delay(random(100, RANDOM_MODE_SPEED));
}

void outputToLights(unsigned char* buffer)
{
    for(int channelIndex=0;channelIndex<CHANNEL_COUNT;channelIndex++){
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

    waitForVixenHeader();
    while (true) {
        int inByte = Serial.read();  // This was "int inByte = Serial1.read(); However, I changed the Vixen serial designation before the setup loop.  May need to play with this.
        if(inByte==-1){
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
        if(index==9 && strcmp(footer,buffer2)==0){
            Serial.println(footer);
            return;
        }
        else if(index==CHANNEL_COUNT){
            outputToLights(buffer);
            index=0;
        }
    }
    Serial.println("");
}

void waitForVixenHeader()
{
    char *header="VIXEN_START";
    char buffer[12];
    int index = 0;
    unsigned long time = millis();

    while (true) {
        int inByte = Serial.read();  // This was "int inByte = Serial1.read();"
        if(inByte==-1){
            if(index==0 && millis()-time>1000) // we haven't read anything in a second
                return;
            continue;
        }
        time = millis();
        buffer[index] = inByte;
        if(buffer[index]!=header[index]) {// not the right sequence restart
            index=-1;
        }
        buffer[index+1] = 0; // add null
        index++;
        if(index==11 && strcmp(header,buffer)==0){
            Serial.println(header);
            return;
        }
    }
}



