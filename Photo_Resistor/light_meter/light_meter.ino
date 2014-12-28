/*
Photoresistor(1kohm resistor(Brown, Black, Red)) is reading light and printing to serialOut.
*/
 
int sensePin = A1;

void setup() {
  analogReference(DEFAULT);
  Serial.begin(9600);  
}

void loop() {
  int reading = analogRead(sensePin);
  Serial.println(reading);
  delay(1000); 
}
