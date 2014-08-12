// Uses a PIR sensor to detect movement, buzzes a buzzer, sends an SMS

#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
 
int ledPin = 5;                // choose the pin for the LED
int inputPin = 6;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int pinSpeaker = 4;          //Set up a speaker on a PWM pin (digital 9, 10, or 11)

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(pinSpeaker, OUTPUT);
  SIM900.begin(19200);
  SIM900power();  
  delay(20000);  // give time to log on to network. 
}

void loop(){
  val = digitalRead(inputPin);  // read input value
  
  if (val == HIGH) { // check if the input is HIGH
    digitalWrite(ledPin, HIGH); // turn LED ON
    for (int i = 0; i < 10; i++) {
      playTone(600, 400);
      delay(100);
    }
    delay(150);
    sendSMS();
    if (pirState == LOW) {
      pirState = HIGH;          // update current state of PIR
    }
  } else {
      digitalWrite(ledPin, LOW); // turn LED off
      playTone(0, 0);
      delay(300);    
      if (pirState == HIGH){
        pirState = LOW;        // update current state of PIR
    }
  }
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

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
}

void sendSMS()
{
  SIM900.print("AT+CMGF=1\r");                                 // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+12252411160\"");              // recipient's mobile number, in international format
  delay(100);
  SIM900.println("Help nigga! They coming to get me!");        // message to send
  delay(100);
  SIM900.println((char)26);                                    // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                                 // give module time to send SMS
  //SIM900power();                                               // turn off module
}


