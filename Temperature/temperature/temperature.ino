// Records temperature using LM35 resistor.

int sensorPin = A0;
int reading;
float voltage;
float temperatureC;
float temperatureF;

void setup()
{
  Serial.begin(9600); //Start the serial connection with the computer
  //to view the result open the serial monitor
  
  analogReference(INTERNAL);
}

void loop() // run over and over again
{
  //getting the voltage reading from the temperature sensor
  reading = analogRead(sensorPin);
  
  // converting that reading to voltage, for 3.3v arduino use 3.3
  voltage = reading * 1.1;
  voltage /= 1024.0;
  
  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");
  
  // print out the temperature
  temperatureC = (voltage - 0.5) * 100 ; //converting from 10 mv per degree with 500 mV offset
  
  // Celsius
  Serial.print(temperatureC); Serial.println(" degrees C");
  
  // Convert to Fahrenheit
  temperatureF = ((temperatureC * 9.0) / 5.0) + 32.0;
  Serial.print(temperatureF); Serial.println(" degrees F");
  
  delay(1000); //waiting a second
}
