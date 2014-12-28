/*
 * Alarm sketch for the doors/windows at mom's house.
 */

#include "pitches.h"

// notes in the melody:
int melody[] = { NOTE_C7, NOTE_C8 };

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 4 };

int switchPin = A0;              // Switch connected to digital pin 2
int ledPin = 9;                  // LED connected at pin 9 via 1k ohm resistor
int epsilon = 1;                 // threshhold for switch, toggle to calibrate

void setup()
{
  Serial.begin(9600);           
  pinMode(switchPin, INPUT);    // sets the digital pin as input to read switch.
  pinMode(ledPin, OUTPUT);      // sets the ledPin to output.
}


void loop()                     
{ 
  int reading = analogRead(switchPin);
  Serial.print("Read switch input: ");
  Serial.println(reading);

  if (reading >= epsilon) 
  {
    digitalWrite(ledPin, HIGH);
    
    // Iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 2; thisNote++) {  
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000/noteDurations[thisNote];
      // speaker hooked up to pin2
      tone(2, melody[thisNote],noteDuration);
      // to distinguish the notes, set a minimum time between them.
      int pauseBetweenNotes = noteDuration * 1.15;
      delay(pauseBetweenNotes);
    }
  } 
  else 
  {
    digitalWrite(ledPin, LOW);
  }
  delay(500);
}

