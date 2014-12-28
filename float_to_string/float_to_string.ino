#include <stdlib.h>

float val = 0.0;
char buff[7];
float results[10];

void setup() 
{  
  Serial.begin(9600);
  
  for (int i = 0; i < 10; i++) 
  {
    results[i] += val;
    dtostrf(val, 4, 2, buff);  //4 is mininum width, 2 is precision
    Serial.print("val: ");
    Serial.println(val);
    val += 5.0;
  }
  
  for (int i = 0; i < 10; i++) 
  {
    Serial.print(results[i]);
    if (i != 9)
    {
      Serial.print(", ");
    }  
  }
  
}

void loop() 
{

}
