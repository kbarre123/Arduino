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
TouchScreenArea startBtn = TouchScreenButton("Start", TSC.createColor(0, 255, 0),     TSC.createColor(0, 0, 0), 125, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea resetBtn = TouchScreenButton("Reset", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 125, TSC.getScreenHeight() - 50, 2, 10);

void setup(void) {
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
      else if(!strcmp(item->getText(),"Start")){
        curMenu = NULL;
        TSC.clearScreen();
        backBtn.draw();
        TSC.drawString("Sensor Data", 50, 50, 2, TSC.createColor(255, 255, 255));
        //TSC.drawString("Sensor Data",unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
        handled = true;
      }
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
      TSC.drawString("Pale Ale", 50, 50, 2, TSC.createColor(255, 255, 255));
      backBtn.draw();
      startBtn.draw();
      //TSC.drawString("Sensor Data",unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
      handled = true;
    }
    else if(!strcmp(item->getText(),"Amber/ESB")){
      curMenu = NULL;
      TSC.clearScreen();
      TSC.drawString("Amber/ESB", 50, 50, 2, TSC.createColor(255, 255, 255));
      backBtn.draw();
      startBtn.draw();
      //TSC.drawString("Sensor Data",unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
      handled = true;
    }
    else if(!strcmp(item->getText(),"Brown/Porter")){
      curMenu = NULL;
      TSC.clearScreen();
      TSC.drawString("Brown/Porter", 50, 50, 2, TSC.createColor(255, 255, 255));
      backBtn.draw();
      startBtn.draw();
      //TSC.drawString("Sensor Data",unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
      handled = true;
    }
    else if(!strcmp(item->getText(),"Stout")){
      curMenu = NULL;
      TSC.clearScreen();
      TSC.drawString("Stout", 50, 50, 2, TSC.createColor(255, 255, 255));
      backBtn.draw();
      startBtn.draw();
      //TSC.drawString("Sensor Data",unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
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
  else if(startBtn.process()){
        curMenu = NULL;
        TSC.clearScreen();
        backBtn.draw();
        TSC.drawString("Sensor Data", 50, 50, 2, TSC.createColor(255, 255, 255));
        //TSC.drawString("Sensor Data",unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
  }
  else if(resetBtn.process()){
        curMenu = NULL;
        TSC.clearScreen();
        backBtn.draw();
        TSC.drawString("Reset Timer", 50, 50, 2, TSC.createColor(255, 255, 255));
        //TSC.drawString("Sensor Data",unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
  }
}

