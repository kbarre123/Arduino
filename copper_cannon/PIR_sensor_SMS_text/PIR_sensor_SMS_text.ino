// Example 55.7
 
#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(7, 8);
 
int led1 = 5;
int inputPin = 6;   // choose the input pin (for PIR sensor)
int pinSpeaker = 4; //Set up a speaker on a PWM pin (digital 9, 10, or 11)
int pirState = LOW; // sensor is 'off'/not being read
//int val = LOW;        // variable for reading the pin status

void setup()
{
  //Serial.begin(19200);
  // set up the digital pins to control
  pinMode(led1, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(pinSpeaker, OUTPUT);
  
  // Set LED to LOW
  digitalWrite(led1, LOW);
  
  SIM900.begin(19200);
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
  //SIM900.println("AT+CMGD=1,4"); // delete all SMS
  //delay(100);
}
 
void loop() 
{
  //If a character comes in from the cellular module...
  if(SIM900.available() >0)
  {
    inchar=SIM900.read(); 
    if (inchar=='O')
    {
      delay(20);
      inchar=SIM900.read(); 
      if (inchar=='N')
      { 
        digitalWrite(led1, HIGH);
        //pirState = HIGH;        // update current state of PIR
        //for (int i = 0; i < 2; i++) 
        //{
        //  playTone(600, 400);
        //  delay(50);
        //}
        SIM900.println("AT+CMGD=1,4"); // delete all SMS
        delay(20);
      } else if (inchar == 'F')  
      {
        //delay(10);
        inchar=SIM900.read();
        if (inchar=='F')
        {
          digitalWrite(led1, LOW);
          //pirState = LOW;
          //for (int i = 0; i < 4; i++) 
          //{
          //  playTone(600, 400);
          //  delay(50);
          //}
          delay(10);
          SIM900.println("AT+CMGD=1,4"); // delete all SMS
        }
      }
    }
  }// END OF AVAILABLE
  
  // READ STATE OF PIR SENSOR
  if (pirState == HIGH) 
  {
    digitalWrite(led1, HIGH); // turn LED on
    for (int i = 0; i < 2; i++) 
    {
      playTone(600, 400);
      delay(100);
    }
    delay(500);
    //val = digitalRead(inputPin);  // read input value
    //if (val == HIGH)
    //{
      //sendSMS();
      //for (int i = 0; i < 2; i++) 
      //{
      //  playTone(600, 400);
      //  delay(50);
      //}
      //delay(150);
    //} else
    //{
      //playTone(0, 0);
      //delay(100);    
    //}
  } else if (pirState == LOW)
  {
    digitalWrite(led1, LOW); // turn LED off
    delay(100);
  }
  
}// END OF MAIN LOOP

// USER DEFINED FUNCTIONS

//  duration in mSecs, frequency in hertz
void playTone(long duration, int freq) 
{
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) 
    {
        digitalWrite(pinSpeaker,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(pinSpeaker, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}
