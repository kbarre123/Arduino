/*
* This sketch demonstrates that state can be maintained through the use of a variable
* (e.g. pirState).
*/

#define ledPin 5
int pirState = LOW; // sensor is 'off'/not being read
int smsData = 1; // Value to represent an SMS message

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}
 
void loop() 
{
  if( smsData > 0 )
  {
    pirState = HIGH;
  }
  else
  {
    pirState = LOW;
  }
  
  if (pirState == HIGH) 
  {
    digitalWrite(ledPin, HIGH);
  } else if (pirState == LOW)
  {
    digitalWrite(ledPin, LOW);
  }
  Serial.println(pirState);
  delay(100);
}// END OF MAIN LOOP

