/*
Here are the hex codes from the generic IR remote that came with my starter kit.
The codes are in order, left to right, top to bottom:
_____________________________
|****************************|
|* Hex Code ||    Button    *|
|***********||***************|
|* B2EEDF3D ||  Power       *|
|* C9C3741  ||  Vol+        *|
|* 2C87261  ||  Func/Stop   *|
|* 1644C1C1 ||  Back        *|
|* A6B913BD ||  Play/Pause  *|
|* D0529225 ||  Forward     *|
|* 1E90961  ||  Down        *|
|* 925D5B5D ||  Vol-        *|
|* CB3D6F7D ||  Up          *|
|* B1EFBA9D ||  0           *|
|* 42640C99 ||  EQ          *|
|* 6BFD8B01 ||  St/Rept     *|
|* DEE522C1 ||  1           *|
|* 6F5974BD ||  2           *|
|* 986FB325 ||  3           *|
|* CBD2CCFD ||  4           *|
|* 25802501 ||  5           *|
|* 85E09D61 ||  6           *|
|* C76EF4E5 ||  7           *|
|* 57E346E1 ||  8           *|
|* 90C35B01 ||  9           *|
|****************************|
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

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
