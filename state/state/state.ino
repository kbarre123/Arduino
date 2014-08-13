/*
* This sketch demonstrates that state can be maintained through the use of a variable
* (e.g. pirState).
*/

#define ledPin 13
int pirState = 0; // sensor is 'off'/not being read
int smsData = 0; // Value to represent an SMS message

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}
 
void loop() 
{
  if( smsData > 0 )
  {
    pirState = 1;
  }
  else
  {
    pirState = 0;
  }
  
  if (pirState == 1) 
  {
    digitalWrite(ledPin, HIGH);
  } else if (pirState == 0)
  {
    digitalWrite(ledPin, LOW);
  }
  Serial.println(pirState);
  delay(100);
}// END OF MAIN LOOP

