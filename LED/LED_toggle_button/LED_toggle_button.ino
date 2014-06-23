/* 
Toggle an LED on/off with a push-button. LED @ pin13. 10K ohm resistor on ground to
prevent switch from bouncing. Connect ground to pin8.
*/

int switchPin = 8;
int ledPin = 13;
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean ledOn = false;

void setup()
{
  //initialize digital pin output
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

boolean debounce(boolean last)
{
  
}

void loop()
{
  if (digitalRead(switchPin) == HIGH && lastButton == LOW)
  {
    ledOn = !ledOn;
    lastButton = HIGH;
  }
  else
  {
    lastButton = digitalRead(switchPin);
  }
  digitalWrite(ledPin, ledOn);
}
