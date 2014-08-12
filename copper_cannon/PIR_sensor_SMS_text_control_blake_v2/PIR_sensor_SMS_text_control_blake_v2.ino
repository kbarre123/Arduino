// Example 55.7
 
#include <SoftwareSerial.h> 
char inchar; // Will hold the incoming character from the GSM shield
SoftwareSerial SIM900(7, 8);
 
int led1 = 5;

void setup()
{
  Serial.begin(19200);
  // set up the digital pins to control
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  
  SIM900.begin(19200);
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
}
 
void loop() 
{
  //If a character comes in from the cellular module...
  if(SIM900.available() >0)
  {
    inchar=SIM900.read(); 
    if (inchar=='O')
    {
      delay(10);
      inchar=SIM900.read(); 
      if (inchar=='N')
      { 
        digitalWrite(led1, HIGH);
      } else if (inchar == 'F')  
      {
        delay(10);
        inchar=SIM900.read();
        if (inchar=='F')
        {
          digitalWrite(led1, LOW);
        }
        SIM900.println("AT+CMGD=1,4"); // delete all SMS
      }
    }
  }
}
