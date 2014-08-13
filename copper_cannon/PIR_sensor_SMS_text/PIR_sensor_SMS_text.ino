// Example 55.7
 
#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(7, 8);

// Define constants 
#define ledPin 5
#define sensorPin 6   // choose the input pin (for PIR sensor)
#define buzzerPin 4   //Set up a speaker on a PWM pin (digital 9, 10, or 11)

// Define variables
int pirState = LOW;  // sensor is 'off'/not being read
//int val = LOW;     // variable for reading the PIR value

void setup()
{
  Serial.begin(9600);
  // set up the digital pins to control
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Set LED to LOW
  digitalWrite(ledPin, LOW);
  
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
  if(SIM900.available() > 0)
  {
    inchar=SIM900.read(); 
    if (inchar=='O')
    {
      delay(20);
      inchar=SIM900.read(); 
      if (inchar=='N')
      { 
        //digitalWrite(ledPin, HIGH);
        pirState = HIGH;
        SIM900.println("AT+CMGD=1,4"); // delete all SMS
        delay(20);
      } else if (inchar == 'F')  
      {
        delay(20);
        inchar=SIM900.read();
        if (inchar=='F')
        {
          //digitalWrite(ledPin, LOW);
          pirState = LOW;
          delay(20);
          SIM900.println("AT+CMGD=1,4"); // delete all SMS
        }
      }
    }
  }// END OF AVAILABLE
  
  // READ STATE OF PIR SENSOR
  if (pirState == HIGH) 
  {
    digitalWrite(ledPin, HIGH); // turn LED on
    
    //val = digitalRead(sensorPin);  // read input value
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
    digitalWrite(ledPin, LOW); // turn LED off
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
        digitalWrite(buzzerPin,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}
