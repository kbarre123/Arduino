/*
This example shows the various abilities of the TouchScreenMenu library.
See the readme.txt file for information on find the libraries this library uses.

 TouchScreenMenu Library. 
 
 2012 Copyright (c) Scott Shaver
 
 Authors: Scott Shaver
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <stdint.h>

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
TouchScreenArea backBtn =  TouchScreenButton("Back",  TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 20, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea resetBtn = TouchScreenButton("Reset", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 125, TSC.getScreenHeight() - 50, 2, 10);

/******************* TEMP SENSOR SETUP *******************/
float tempUpper = 0;
float tempMash = 0;

/******************* TIMER SETUP *************************/
long interval = 1000;  // Threshold at which to update the Timer
unsigned long currentMillis = 0;  // current millis
unsigned long previousMillis = 0;  // Will store last time Timer was updated
unsigned long benchMillis = 0;  // Variable used to be able to reset second to zero and still be able to keep track of time
int second = 0, minute = 0, hour = 0; // Keep track of time so we can increment min/hours based on elapsed seconds
const long MILLIS_IN_MINUTE = 60000;  // Constant to calculate printable time
char _hour[3]; // Buffer to store the current hour in. Used to compare to see if it has changed and needs to be updated. Prevents blinking every pass through main loop.
char _minute[3];
char _second[3];

void setup(void) {
  Serial.begin(9600);
  previousMillis = millis();
  TSC.setBackColor(TSC.createColor(0, 0, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized
  curMenu->draw(); // put up the main menu
}

void loop(void) {
  // handle the current menu
  if(curMenu!=NULL){
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
  }else{
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them were pressed
    checkButtons();
  }
}

// check to see if any menu item was pressed and do something
void checkMenuSelection(TouchScreenMenuItem *item) {
  boolean handled = false;
  if(item != NULL){
    // main menu items 
    if(curMenu == &mainMenu){
      if(!strcmp(item->getText(),"Water Chem")){
        curMenu = &waterMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      // THIS IS WHERE ALL THE MAGIC HAPPENS------------------------------------------------------------------------------------>
      else if(!strcmp(item->getText(),"Start")){
        curMenu = NULL;
        TSC.clearScreen();
        
        // Convert sensor data to strings for display.
        char textUpper[8];                    // buffer to store the results of dtostrf
        dtostrf(tempUpper, 1, 2, textUpper);  // Arguments are (float, width, precision, buffer)
        char textMash[8];
        dtostrf(tempMash, 1, 2, textMash);
        
        // Display results
        TSC.drawString("Upper:", 20, 20, 2, TSC.createColor(255, 255, 255));
        TSC.drawString(textUpper, 150, 20, 2, TSC.createColor(255, 255, 255));
        Serial.print("textUpper: ");  // DEBUG
        Serial.println(textUpper);  // DEBUG
        
        TSC.drawString("Mash:", 20, 50, 2, TSC.createColor(255, 255, 255));
        TSC.drawString(textMash, 150, 50, 2, TSC.createColor(255, 255, 255));
        Serial.print("textMash: ");  // DEBUG
        Serial.println(textMash);  // DEBUG
        Serial.println("");  // DEBUG
        
        backBtn.draw();
        resetBtn.draw();
        handled = true;
      }// THIS IS WHERE ALL THE MAGIC HAPPENS----------------------------------------------------------------------------------->
    }
    else if(curMenu == &waterMenu){
      if(!strcmp(item->getText(),"Back")){
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Pale Ale")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Pale Ale    5gal  10gal", 20, yPos,       1, TSC.createColor(255, 255, 255));
        TSC.drawString("Dilution:    50%    50%", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Gypsum:     7.0g  14.0g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Salt:       1.2g   2.4g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("CaCl2:      1.0g   2.0g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Pick Lime:  0.2g   0.4g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("MgCl2:      1.0g   2.0g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Amber/ESB")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Amber/ESB    5gal   10gal", 20, yPos,       1, TSC.createColor(255, 255, 255));
        TSC.drawString("Dilution:      0%      0%", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Gypsum:     1.80g   3.60g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Salt:       0.00g   0.00g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("CaCl2:      1.20g   2.40g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Phos Acid:  0.80g   1.60g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("MgCl2:      1.12g   2.24g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Brown/Porter")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Brn/Prtr     5gal   10gal", 20, yPos,       1, TSC.createColor(255, 255, 255));
        TSC.drawString("Dilution:     50%     50%", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Gypsum:     1.80g   3.60g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Pick Lime:  0.40g   0.80g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("CaCl2:      0.80g   1.60g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Phos Acid:  0.80g   1.60g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("MgCl2:      1.12g   2.24g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        backBtn.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Stout")){
        curMenu = NULL;
        TSC.clearScreen();
        int yPos = 20;
        TSC.drawString("Stout       5gal  10gal", 20, yPos,       1, TSC.createColor(255, 255, 255));
        TSC.drawString("Dilution:    25%    25%", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Gypsum:     0.8g   1.6g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Salt:       0.4g   0.8g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("CaCl2:      0.4g   0.8g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Phos Acid:  1.0g   2.0g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("Pick Lime:  0.8g   1.6g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        TSC.drawString("MgCl2:      1.0g   2.0g", 20, yPos += 20, 1, TSC.createColor(255, 255, 255));
        backBtn.draw();
        handled = true;
      }
    }
    
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
        curMenu->drawItem(item,false);
    }
}

// check various buttons and perform actions if any was pressed
void checkButtons(){
  if(backBtn.process()){ // return from the graphics function screen
    curMenu = &waterMenu;
    TSC.clearScreen();
    curMenu->draw();
  }
  else if(resetBtn.process()){
    // Reset timer
  }
}

