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

/* MENUS: All menus are declared here. */
// create the array of items for the main menu
TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("5 gallons"),
  TouchScreenMenuItem("10 gallons"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the 5 gallons sub menu
TouchScreenMenuItem fiveGalMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Water Chem"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the 10 gallons sub menu
TouchScreenMenuItem tenGalMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Water Chem"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the water chemistry sub menu
TouchScreenMenuItem waterChemMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Pale Ale"),
  TouchScreenMenuItem("Amber/ESB"),
  TouchScreenMenuItem("Brown/Porter"),
  TouchScreenMenuItem("Stout"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the Pale Ale menu
TouchScreenMenuItem paleAleMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the Pale Ale menu
TouchScreenMenuItem amberESBMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the Pale Ale menu
TouchScreenMenuItem brownPorterMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the Pale Ale menu
TouchScreenMenuItem stoutMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Start"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the start sub menu
TouchScreenMenuItem startMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("ENDOFMENU")
};/* END OF MENU ITEMS */

// create the various menus setting the items, font size, spacing, padding, justification and titles
TouchScreenMenu mainMenu =        TouchScreenMenu(mainMenuItems,        2, 10, 10, CENTERJ, "Main Menu");
TouchScreenMenu fiveGalMenu =     TouchScreenMenu(fiveGalMenuItems,     2, 10, 5,  CENTERJ, "5 gallons");
TouchScreenMenu tenGalMenu =      TouchScreenMenu(tenGalMenuItems,      2, 10, 5,  CENTERJ, "10 gallons");
TouchScreenMenu waterChemMenu =   TouchScreenMenu(waterChemMenuItems,   2, 10, 5,  CENTERJ, "Water Chem");
TouchScreenMenu paleAleMenu =     TouchScreenMenu(paleAleMenuItems,     2, 10, 5,  CENTERJ, "Pale Ale");
TouchScreenMenu amberESBMenu =    TouchScreenMenu(amberESBMenuItems,    2, 10, 5,  CENTERJ, "Amber/ESB");
TouchScreenMenu brownPorterMenu = TouchScreenMenu(brownPorterMenuItems, 2, 10, 5,  CENTERJ, "Brown/Porter");
TouchScreenMenu stoutMenu =       TouchScreenMenu(stoutMenuItems,       2, 10, 5,  CENTERJ, "Stout");
TouchScreenMenu startMenu =       TouchScreenMenu(startMenuItems,       2, 10, 5,  CENTERJ, "Start");

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &mainMenu;

// DECLARE TOUCH AREAS FOR BUTTONS
// Main Menu buttons
TouchScreenArea fiveGalBtn =         TouchScreenButton("5 Gal",        TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea tenGalBtn =          TouchScreenButton("10 Gal",       TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 100, 2, 10);

// 5 Gallon buttons
TouchScreenArea backFromFiveGalBtn = TouchScreenButton("<- Back",      TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea paleAleBtn =         TouchScreenButton("Pale Ale",     TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 75, 2, 10);
TouchScreenArea amberESBBtn =        TouchScreenButton("Amber/ESB",    TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 100, 2, 10);
TouchScreenArea brownPorterBtn =     TouchScreenButton("Brown/Porter", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 125, 2, 10);
TouchScreenArea stoutBtn =           TouchScreenButton("Stout",        TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 150, 2, 10);

// 10 Gallon buttons; note that this menu uses the same beer buttons as the 5 gallon menu so no need to declare new ones
TouchScreenArea backFromTenGalBtn =  TouchScreenButton("<- Back",      TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);

// Start button
TouchScreenArea startBtn =           TouchScreenButton("Start",        TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);

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
    // to see if any of them was pressed
    checkButtons();
  }
}

// check various buttons and perform actions if any were pressed
void checkButtons(){
  if(fiveGalBtn.process()){ // return from the 5 gallon screen
        curMenu = &fiveGalMenu;
        TSC.clearScreen();
        curMenu->draw();
  }
  else if(tenGalBtn.process()){ // return from the 10 gallon screen
        curMenu = &tenGalMenu;
        TSC.clearScreen();
        curMenu->draw();
  }
  else if(backFromFiveGalBtn.process()){ // return from the 10 gallon screen
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
  }
  else if(paleAleBtn.process()){ // return from the 10 gallon screen
      curMenu = &paleAleMenu;
      TSC.clearScreen();
      curMenu->draw();
  }
  else if(amberESBBtn.process()){ // return from the 10 gallon screen
      curMenu = &amberESBMenu;
      TSC.clearScreen();
      curMenu->draw();
  }
  else if(brownPorterBtn.process()){ // return from the 10 gallon screen
      curMenu = &brownPorterMenu;
      TSC.clearScreen();
      curMenu->draw();
  }
  else if(stoutBtn.process()){ // return from the 10 gallon screen
      curMenu = &stoutMenu;
      TSC.clearScreen();
      curMenu->draw();
  }
  else if(startBtn.process()){ // return from the 10 gallon screen
      curMenu = &startMenu;
      TSC.clearScreen();
      curMenu->draw();
  }
}

// check to see if any menu item was pressed and do something
void checkMenuSelection(TouchScreenMenuItem *item) {
  boolean handled = false;
  if(item != NULL){
    // MAIN MENU ITEMS
    if(curMenu == &mainMenu){
      if(!strcmp(item->getText(),"5 gallons")){
        curMenu = &fiveGalMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"10 gallons")){
        curMenu = &tenGalMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    
    // 5 GALLON MENU
    else if(curMenu == &fiveGalMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Water Chem")){
        curMenu = &waterChemMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Start")){
        curMenu = &startMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // 10 GALLON MENU
    else if(curMenu == &tenGalMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Water Chem")){
        curMenu = &waterChemMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Start")){
        curMenu = &startMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    
    // WATER CHEM MENU
    else if(curMenu == &waterChemMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Pale Ale")){
        curMenu = &paleAleMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Amber/ESB")){
        curMenu = &amberESBMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Brown/Porter")){
        curMenu = &brownPorterMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Stout")){
        curMenu = &stoutMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // PALE ALE MENU
    else if(curMenu == &paleAleMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &waterChemMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Start")){
        curMenu = &startMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // AMBER/ESB MENU
    else if(curMenu == &amberESBMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &waterChemMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Start")){
        curMenu = &startMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // BROWN/PORTER MENU
    else if(curMenu == &brownPorterMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &waterChemMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Start")){
        curMenu = &startMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // STOUT MENU
    else if(curMenu == &stoutMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &waterChemMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Start")){
        curMenu = &startMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // START MENU
    else if(curMenu == &startMenu){
      curMenu = &startMenu;
      TSC.clearScreen();
      curMenu->draw();
      handled = true;
    }
    
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
        curMenu->drawItem(item,false);
  }
}
