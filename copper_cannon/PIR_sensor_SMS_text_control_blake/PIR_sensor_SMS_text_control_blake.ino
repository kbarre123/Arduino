// Example 55.7
 
#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(7, 8);
 
int ledPin = 5;       // choose the pin for the LED
int inputPin = 6;   // choose the input pin (for PIR sensor)
int pinSpeaker = 4; //Set up a speaker on a PWM pin (digital 9, 10, or 11)
int pirState = LOW; // sensor is 'off'/not being read
int val = LOW;        // variable for reading the pin status
 
void setup()
{
  Serial.begin(19200);
  // set up the digital pins to control
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(pinSpeaker, OUTPUT);
  
  // Set LED to LOW
  digitalWrite(ledPin, LOW);
 
  // wake up the GSM shield
  //SIM900power(); 
  SIM900.begin(19200);
  delay(20000);  // give time to log on to network.
  // DEBUG: Beep to confirm ready status
  for (int i = 0; i < 2; i++) 
  {
    playTone(600, 400);
    delay(500);
  }
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
  //Serial.println("Ready...");
}
 
void loop() 
{  
  //If a character comes in from the cellular module...
  if(SIM900.available() > 0)
  {
    inchar=SIM900.read(); 
    if (inchar=='A')
    {
      delay(10);
      inchar=SIM900.read(); 
      if (inchar=='R')
      {
        delay(10);
        inchar=SIM900.read();
        if (inchar=='M')
        {
          pirState = HIGH;        // update current state of PIR
          for (int i = 0; i < 2; i++) 
          {
            playTone(600, 400);
            delay(500);
          }
        } // END 'M'
      } // END 'R'
    }
    SIM900.println("AT+CMGD=1,4"); // delete all SMS
  } // END 'available()'
  
  // Read state of PIR switch
  if (pirState == HIGH) 
  {
    digitalWrite(ledPin, HIGH); // turn LED ON
    val = digitalRead(inputPin);  // read input value
    if (val == HIGH) 
    { // check if the input is HIGH
      //sendSMS();
      for (int i = 0; i < 10; i++) 
      {
        playTone(600, 400);
        delay(500);
      }
      delay(150);
    } 
  } else if (pirState == LOW)
  {
    digitalWrite(ledPin, LOW); // turn LED off
    playTone(0, 0);
    delay(100);
  }
}// END OF MAIN LOOP

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}

//  duration in mSecs, frequency in hertz
void playTone(long duration, int freq) {
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(pinSpeaker,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(pinSpeaker, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}

void sendSMS()
{
  SIM900.print("AT+CMGF=1\r");                                 // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+12252411160\"");              // recipient's mobile number, in international format
  delay(100);
  SIM900.println("Test");        // message to send
  delay(100);
  SIM900.println((char)26);                                    // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                                 // give module time to send SMS
  //SIM900power();                                               // turn off module
}
