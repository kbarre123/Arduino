#include "Pitches.h"

//**************** TONE DECLARATIONS ****************
int melody[] = {
  NOTE_CS5; 
};

int noteDurations[] = {
  6 
};

//**************** LED DECLARATIONS *****************
int ledPin1 = 7;      //Location of LED 1, eye1

//**************** PIR DECLARATIONS *****************
int motionPin = 2;
int pirState = LOW;
int val = 0;           // Variable for reading PIR status

void setup(){
  pinMode(ledPin1, OUTPUT);    // Initialize digital pin output
  pinMode(motionPin, INPUT);    // Declare sensor as input
}

void loop() {

  val = digitalRead(motionPin);  // Read input value
  
  if (val == HIGH) {
    
    // Iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 1; thisNote++) {
      // to calculate the note duration, take one second 
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000/noteDurations[thisNote];
      tone(4, melody[thisNote],noteDuration);
    
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.5;
      delay(pauseBetweenNotes);
      
      digitalWrite(ledPin1, HIGH);
    }
  delay(1000);
}
el
}

