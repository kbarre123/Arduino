//#include <TouchScreenMenu.h>     // https://code.google.com/p/touch-screen-menu-library/
#include <TouchScreen.h>         // https://github.com/adafruit/Touch-Screen-Library
#include <TFT.h>
#include <cstddef.h>
#include <stdint.h>

#ifdef MEGA
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3
 
#else           // Used for Uno
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3 

#endif

int count = 0;
int prevCount = 0;
int countdigits[] = { 0, 0, 0 };
int prevdigits[] =  { 0, 0, 0 };
int x = 0;
#define kerning 13 // px between digits

void setup() 
{
  Serial.begin(9600);
  Tft.init();
  Tft.setDisplayDirect(UP2DOWN);
  Tft.paintScreenBlack();
}

void loop() 
{ 
  count++; // Increment the count

  //Grab the last digit of the speed
  countdigits[2] = count % 10;
  
  //How to handle the middle digit depends on if the
  //the speed is a two or three digit number
  if(count > 99)
  {
    countdigits[1] = (count / 10) % 10;
  } else
  { 
    countdigits[1] = count / 10;
  }
  
  //Grab the first digit
  countdigits[0] = count / 100;
 
  //Split out the digits of the previous speed
  prevdigits[2] = prevCount % 10;
  if(prevCount > 99)
  {
    prevdigits[1] = (prevCount / 10) % 10;
  } else
  {
   prevdigits[1] = prevCount / 10;
  }
  prevdigits[0] = prevCount / 100;  

  Serial.print(countdigits[0]);
  Serial.print(" ");
  Serial.print(countdigits[1]);
  Serial.print(" ");
  Serial.print(countdigits[2]);
  Serial.print("  :  ");
  Serial.print(prevdigits[0]);
  Serial.print(" ");
  Serial.print(prevdigits[1]);
  Serial.print(" ");
  Serial.print(prevdigits[2]);
  Serial.println("");
  
  //Now print the digits on the TFT screen.
  //Only execute this block if the speed has changed.
  if(count != prevCount) {
    //Compare each digit to the value from the previous loop.
    //The digit will only be redrawn if it has changed.
    for(x = 0; x < 3; x++)
    {
      if(countdigits[x] != prevdigits[x])
      {
        // Cast ints to strings for display
        char prevText[8];
        dtostrf(prevdigits[x], 1, 0, prevText);
        char currentText[8];
        dtostrf(countdigits[x], 1, 0, currentText);

        //Black out old value first. 
        //Draw digit in black over the top of white digit
        Tft.drawString(prevText, 140, (kerning * x), 2, BLACK);
        //print new value in white
        if((x == 0) and (count > 99) and (countdigits[x] > 0))
        {
          Tft.drawString(currentText, 140, (kerning * x), 2, WHITE);
        }
        if((x == 1) and (count >= 99))
        {
          Tft.drawString(currentText, 140, (kerning * x), 2, WHITE);
        } else if ((x == 1) and (count < 99) and (countdigits[x] > 0))
        {
          Tft.drawString(currentText, 140, (kerning * x), 2, WHITE);
        }
        if(x == 2)
        {
          Tft.drawString(currentText, 140, (kerning * x), 2, WHITE);
        }
      }
    }
  }
  delay(999); //Delay screen updates to simulate 1 second pulse counting.
  prevCount = count; //Store current speed for comparison on the next loop.
}
