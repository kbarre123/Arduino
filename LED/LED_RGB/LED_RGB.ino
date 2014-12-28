/*
Adafruit Arduino - Lesson 3. RGB LED
Note: Radio Shack sells common anode versions. So, with the flat side to the left, the pin-out is as follows:
    ______
   |      |
   |      |
  _|      |_
 |__________)
   | |  | |
   | |  | |
   | |  | |
   | |  | |
   | |  | |
   R A* B G
   E    L R
   D    U E
        E E
          N
* A* = Common Anode to +5V
* Red: 220 Ohm resistor
* Blue/Green: 100 Ohm resistor
* All 3 pins must be on a PWM pin to work    
*/

int redPin = 11;
int greenPin = 10;
int bluePin = 9;

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop()
{
  //analogWrite(redPin, 255);
  setColor(255, 0, 0);  // red
  delay(1000);
  setColor(0, 255, 0);  // green
  delay(1000);
  setColor(0, 0, 255);  // blue
  delay(1000);
  setColor(255, 255, 0);  // yellow
  delay(1000);  
  setColor(80, 0, 80);  // purple
  delay(1000);
  setColor(0, 255, 255);  // aqua
  delay(1000);
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
