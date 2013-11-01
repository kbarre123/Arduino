#include "pitches.h"

//**************** TONE DECLARATIONS ****************
int melody[] = {  // Notes in the melody:
  NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_D5, NOTE_FS4,
  NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_D5, NOTE_FS4, 
  NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_D5, NOTE_FS4,
  NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_D5, NOTE_FS4, 
  
  NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_CS5, NOTE_F4,
  NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_CS5, NOTE_F4,
  NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_CS5, NOTE_F4,
  NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_F4, NOTE_C5, NOTE_F4, NOTE_CS5, NOTE_F4,
 };

int noteDurations[] = {  // Note durations: 4 = quarter note, 8 = eighth note, etc.:
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
};

//**************** LED DECLARATIONS *****************
int ledPin1 = 3;      //Location of LED 1, eye1
int ledPin2 = 13;     //Location of LED 2, eye2
int ledPin3 = 1;      //Location of LED 3, ambient light

//**************** PIR DECLARATIONS *****************
int motionPin = 2;      //Input pin for PIR sensor
int pirState = LOW;    // Initial state of PIR sensor
int val = 0;           // Variable for reading PIR status

void setup(){
  pinMode(ledPin1, OUTPUT);    // Initialize digital pin output
  pinMode(ledPin2, OUTPUT);    // Initialize digital pin output
  pinMode(ledPin3, OUTPUT);    // Initialize digital pin output
  pinMode(motionPin, INPUT);    // Declare sensor as input
}

void loop() {

  val = digitalRead(motionPin);  // Read input value
  
  if (val == HIGH) {
    
    // Iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 80; thisNote++) {  
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(4, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    
    // turn on green LED
    digitalWrite(ledPin3, HIGH);
    
    // blink red LED's on alternating notes
    if(thisNote % 2 == 0) {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
    } else {
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
      }
    }
    
    // Update current state of PIR
    if (pirState == LOW) {         
      pirState = HIGH;
    }
    
  } else {
    digitalWrite(ledPin1, HIGH);  
    digitalWrite(ledPin2, HIGH);  
    digitalWrite(ledPin3, HIGH);  
    delay(300);
    // Update current state of PIR
    if(pirState == HIGH) {         
      pirState = LOW;              
    }
  }
}

