// Uses a PIR sensor to detect movement, buzzes a buzzer
 
int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
//int pinSpeaker = 10;           //Set up a speaker on a PWM pin (digital 9, 10, or 11)

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  //pinMode(pinSpeaker, OUTPUT);
}

void loop(){
  val = digitalRead(inputPin);  // read input value
  
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH); // turn LED ON
    //playTone(300, 160);
    delay(150);
    if (pirState == LOW) {
      pirState = HIGH;          // update current state of PIR
    }
  } else {
      digitalWrite(ledPin, LOW); // turn LED off
      //playTone(0, 0);
      delay(300);    
      if (pirState == HIGH){
        pirState = LOW;        // update current state of PIR
    }
  }
}
/* duration in mSecs, frequency in hertz
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
}*/

