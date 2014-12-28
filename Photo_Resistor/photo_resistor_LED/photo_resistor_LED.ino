/*
Photoresistor is reading light and printing to serialOut.
Potentiometer is controlling the brightness of an LED.
The LED and photoresistor's lead are bent so that they're face to face, almost touching.
The whole unit is in a box to prevent outside light sources from interfering with the reading.
*/

//pot
int sensor = A0;
int ledPin = 9; 
//photo
int potPin = A5;

void setup() {
  //pot
  pinMode(ledPin, OUTPUT);
  //photo
  analogReference(DEFAULT);
  Serial.begin(9600);  
}

void loop() { 
  //photo
  Serial.println(analogRead(potPin));
  delay(1000);
  //pot
  int reading = analogRead(sensor);
  int bright = reading / 4;                                          
  analogWrite(ledPin, bright); 
}
