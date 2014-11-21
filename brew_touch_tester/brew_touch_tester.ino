/*
  There's a fundamental difference b/t a 'Menu item' and a 'button'.
  While they look the same, they are handled differently by the TSC library. This is why this program
  tests for menu first, then tests to see if a button has been pressed. They look the same on the
  screen, but there's a heirarchy here and menus come first.
*/

#include <cstddef.h>
#include <stdint.h>

#include <TouchScreenMenu.h>     // https://code.google.com/p/touch-screen-menu-library/
#include <TouchScreen.h>         // https://github.com/adafruit/Touch-Screen-Library
#include <TFT.h>

#include <OneWire.h>             // Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h>   // Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <Pitches.h>             // Taken from the Arduino website

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

/******************* MENU SETUP ***********************/
// create the array of items for the main menu
TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("Water Chem"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the first sub menu
TouchScreenMenuItem waterItems[] = {
  TouchScreenMenuItem("Back"),
  TouchScreenMenuItem("Pale Ale"),
  TouchScreenMenuItem("Amber/ESB"),
  TouchScreenMenuItem("Brown/Porter"),
  TouchScreenMenuItem("Stout"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the various menus setting the items, font size, spacing, padding, justification and titles
TouchScreenMenu waterMenu = TouchScreenMenu(waterItems, 2, 15, 10, CENTERJ, "Water Chem");
TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 20, 20, CENTERJ, "Main Menu");

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &mainMenu;

// Buttons for non-menu screens that are only monitored if curMenu=NULL (i.e. not on a menu screen)
TouchScreenArea backBtn =  TouchScreenButton("Back",  WHITE, BLACK, 20, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea resetBtn = TouchScreenButton("Reset", WHITE, BLACK, 125, TSC.getScreenHeight() - 50, 2, 10);

/******************* TIMER SETUP *************************/
int count = 0;
int prevCount = 0;
int countdigits[] = { 0, 0, 0 };
int prevdigits[] =  { 0, 0, 0 };
int x = 0;
#define kerning 13 // px between digits

/******************* MAIN SETUP **************************/
void setup() 
{
  Serial.begin(9600);
  Tft.init();
  //Tft.setOrientation(0);
  //Tft.setDisplayDirect(UP2DOWN);
  Tft.paintScreenBlack();

  // Scott's menu init stuff
  TSC.init();
  TSC.setBackColor(BLACK);
  curMenu->draw();
}

/******************** MAIN LOOP***************************/
void loop() 
{ 
  Serial.println("Enter loop"); // DEBUG
  
  /* UDATE TIMER */
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

  Serial.print(countdigits[0]);  // DEBUG
  Serial.print(" ");             // DEBUG
  Serial.print(countdigits[1]);  // DEBUG
  Serial.print(" ");             // DEBUG
  Serial.print(countdigits[2]);  // DEBUG
  Serial.print("  :  ");         // DEBUG
  Serial.print(prevdigits[0]);   // DEBUG
  Serial.print(" ");             // DEBUG
  Serial.print(prevdigits[1]);   // DEBUG
  Serial.print(" ");             // DEBUG
  Serial.print(prevdigits[2]);   // DEBUG
  Serial.println("");            // DEBUG
  
  //Only execute this block if the count has changed.
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
  /* END UPDATE TIMER */

  /* TEST CURRENT MENU */
  if(curMenu!=NULL)
  {
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
  } else
  {
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them were pressed
    checkButtons();
  }
  /* END TEST CURRENT MENU */
  
  Serial.println("Exit loop"); // DEBUG
  Serial.println("");          // DEBUG
}

/******************** FUNCTIONS ***************************/

// check to see if any menu item was pressed and do something
void checkMenuSelection(TouchScreenMenuItem *item)
{
  //Serial.println("Enter checkMenuSelection()"); // DEBUG
  boolean handled = false;
  if(item != NULL)
  {
    // main menu items 
    if(curMenu == &mainMenu)
    {
      if(!strcmp(item->getText(),"Water Chem"))
      {
        curMenu = &waterMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      // Main Display
      else if(!strcmp(item->getText(),"Start"))
      {
        curMenu = NULL;
        TSC.clearScreen();
        TSC.drawString("Upper:", 20, 20, 2, WHITE);
        TSC.drawString("Mash:", 20, 50, 2, WHITE);
        TSC.drawString(":", 50, 80, 2, WHITE);
        TSC.drawString(":", 95, 80, 2, WHITE);
        
        // Turn on timer display in main()
        //displaySensorSwitch = true;
        
        // Draw buttons
        backBtn.draw();
        resetBtn.draw();
        handled = true;
      }
    }
    else if(curMenu == &waterMenu)
    {
      if(!strcmp(item->getText(),"Back"))
      {
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Pale Ale"))
      {
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Pale Ale    5gal  10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:    50%    50%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     7.0g  14.0g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Salt:       1.2g   2.4g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Pick Lime:  0.2g   0.4g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Amber/ESB"))
      {
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Amber/ESB    5gal   10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:      0%      0%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     1.80g   3.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Salt:       0.00g   0.00g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      1.20g   2.40g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Phos Acid:  0.80g   1.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.12g   2.24g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Brown/Porter"))
      {
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Brn/Prtr     5gal   10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:     50%     50%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     1.80g   3.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Pick Lime:  0.40g   0.80g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      0.80g   1.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Phos Acid:  0.80g   1.60g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.12g   2.24g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Stout"))
      {
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Stout       5gal  10gal", 20, yPos,       1, WHITE);
        TSC.drawString("Dilution:    25%    25%", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Gypsum:     0.8g   1.6g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Salt:       0.4g   0.8g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("CaCl2:      0.4g   0.8g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Phos Acid:  1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("Pick Lime:  0.8g   1.6g", 20, yPos += 20, 1, WHITE);
        TSC.drawString("MgCl2:      1.0g   2.0g", 20, yPos += 20, 1, WHITE);
        backBtn.draw();
        handled = true;
      }
    }
    
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
      curMenu->drawItem(item,false);
    }
  //Serial.println("Exit checkMenuSelection()"); // DEBUG
}

// check various buttons and perform actions if any was pressed
void checkButtons()
{
  //Serial.println("Enter checkButtons()"); // DEBUG
  if(backBtn.process())
  {
    //displaySensorSwitch = false;
    curMenu = &mainMenu;
    TSC.clearScreen();
    curMenu->draw();
  }
  else if(resetBtn.process())
  {
    // Reset timer
  }
  //Serial.println("Exit checkButtons()"); // DEBUG
}

