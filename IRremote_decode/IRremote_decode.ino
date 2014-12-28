/*
This sketch reads the code from the IR remote and prints which button you just pressed.
Here are the hex and raw codes from the generic IR remote that came with my starter kit.
The codes are in order, left to right, top to bottom:
_________________________________________
|****************************************|
|* Hex Code ||  Raw Data  ||  Button    *|
|***********||***************************|
|* B2EEDF3D || 3001999165 ||Power       *|
|* C9C3741  || 211564353  ||Vol+        *|
|* 2C87261  || 46690913   ||Func/Stop   *|
|* 1644C1C1 || 373604801  ||Back        *|
|* A6B913BD || 2797147069 ||Play/Pause  *|
|* D0529225 || 3495072293 ||Forward     *|
|* 1E90961  || 32049505   ||Down        *|
|* 925D5B5D || 2455591773 ||Vol-        *|
|* CB3D6F7D || 3409801085 ||Up          *|
|* B1EFBA9D || 2985278109 ||0           *|
|* 42640C99 || 1113853081 ||EQ          *|
|* 6BFD8B01 || 1811778305 ||St/Rept     *|
|* DEE522C1 || 3739558593 ||1           *|
|* 6F5974BD || 1868133565 ||2           *|
|* 986FB325 || 2557457189 ||3           *|
|* CBD2CCFD || 3419589885 ||4           *|
|* 25802501 || 629155073  ||5           *|
|* 85E09D61 || 2246090081 ||6           *|
|* C76EF4E5 || 3345937637 ||7           *|
|* 57E346E1 || 1474512609 ||8           *|
|* 90C35B01 || 2428721921 ||9           *|
|****************************************|
*/
#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() 
{
  if (irrecv.decode(&results)) 
  {
    // Print the values to monitor. This value is the "repeat" value if the button is depressed for too long.
    // It is considered noise in this program. So, if the value is anything other than noise, do the following:
    //Serial.println(results.value);  
    
    if (results.value == 3001999165)
    {
      Serial.println("You pressed the power button!");
    } 
    else if (results.value == 211564353)
    {
      Serial.println("You pressed the Vol+ button!");  
    }
    else if (results.value == 46690913)
    {
      Serial.println("You pressed the Func/Stop button!");  
    }
    else if (results.value == 373604801)
    {
      Serial.println("You pressed the Back button!");  
    }
    else if (results.value == 2797147069)
    {
      Serial.println("You pressed the Pause button!");  
    }
    else if (results.value == 3495072293)
    {
      Serial.println("You pressed the Forward button!");  
    }
    else if (results.value == 32049505)
    {
      Serial.println("You pressed the Down button!");  
    }
    else if (results.value == 2455591773)
    {
      Serial.println("You pressed the Vol- button!");  
    }
    else if (results.value == 3409801085)
    {
      Serial.println("You pressed the Up button!");  
    }
    else if (results.value == 2985278109)
    {
      Serial.println("You pressed the 0 button!");  
    }
    else if (results.value == 1113853081)
    {
      Serial.println("You pressed the EQ button!");  
    }
    else if (results.value == 1811778305)
    {
      Serial.println("You pressed the St/Rept button!");  
    }
    else if (results.value == 3739558593)
    {
      Serial.println("You pressed the 1 button!");  
    }
    else if (results.value == 1868133565)
    {
      Serial.println("You pressed the 2 button!");  
    }
    else if (results.value == 2557457189)
    {
      Serial.println("You pressed the 3 button!");  
    }
    else if (results.value == 3419589885)
    {
      Serial.println("You pressed the 4 button!");  
    }
    else if (results.value == 629155073)
    {
      Serial.println("You pressed the 5 button!");  
    }
    else if (results.value == 2246090081)
    {
      Serial.println("You pressed the 6 button!");  
    }
    else if (results.value == 3345937637)
    {
      Serial.println("You pressed the 7 button!");  
    }
    else if (results.value == 1474512609)
    {
      Serial.println("You pressed the 8 button!");  
    }
    else if (results.value == 2428721921)
    {
      Serial.println("You pressed the 9 button!");  
    }
    
    // Receive the next value
    irrecv.resume(); 
  }
}
