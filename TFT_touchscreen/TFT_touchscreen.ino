// Draw Texts in the four directions of the TFT by Frankie.Chu
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/*
    Modified record:
    
*/
#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>
#include <math.h>

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

/* DEBUG: These variables are temp and represent output of the DallasTemp sensors,
 * as converted from floats to char[] for display to the screen.
*/
float tempUpper = 125.65;
float tempMash = 75.22;

// Variables for the timer
long interval = 1000;  // Threshold at which to update the Timer
long previousMillis = 0;  // Will store last time Timer was updated
unsigned long currentMillis;  // current millis
long hours=0;
long mins=0;
long secs=0;
long leadingTenThreshold = 10;

// Constants to calculate printable time
const long MILLIS_IN_HOUR= 3600000;
const long MILLIS_IN_MINUTE = 60000;
const long MILLIS_IN_SECOND = 1000;

void setup()
{
  Serial.begin(9600);
  Tft.init();  //init TFT library
  Tft.paintScreenBlack();  // Clear screen
  delay(1000);
  
  Tft.setDisplayDirect(UP2DOWN);
  Tft.drawString("Upper: ",220,20,2,WHITE);
  Tft.drawString("Mash: ",180,20,2,WHITE);
  Tft.drawString("Timer: ",140,20,2,WHITE);
  Tft.fillRectangle(10, 20, 50, 125, GREEN);
  Tft.fillRectangle(10, 175, 50, 125, RED);
  Tft.drawString("Start",45,27,3,BLACK);
  Tft.drawString("Stop",45,193,3,BLACK);
}

void loop()
{
  
  currentMillis = millis();
  /* READ SENSORS AND DISPLAY */
  // Convert sensor data of Upper Pot to string for display. 
  char textUpper[8]; // buffer to store the results of dtostrf
  dtostrf(tempUpper, 1, 2, textUpper);  // Arguments are (float, width, precision, buffer)
  // Convert sensor data of Mash Tun to string for display
  char textMash[8];  // buffer to store the results of dtostrf
  dtostrf(tempMash, 1, 2, textMash);
  // Display results
  Tft.drawString(textUpper,220,180,2,WHITE);
  //Serial.print("textUpper: ");  // DEBUG
  //Serial.println(textUpper);  // DEBUG
  Tft.drawString(textMash,180,180,2,WHITE);
  //Serial.print("textMash: ");  // DEBUG
  //Serial.println(textMash);  // DEBUG
  //Serial.println("");  // DEBUG
  
  /* UPDATE TIMER */
  if(currentMillis - previousMillis > interval) 
  {
    // save the last time you updated Timer
    previousMillis = currentMillis;
    
    secs = floor(currentMillis / 1000);
    char textSecs[4];
    dtostrf(secs,1,0,textSecs);
    Serial.print("textSecs: ");
    Serial.println(textSecs);
    
    mins = floor(secs / 60);
    char textMins[3];
    dtostrf(mins,1,0,textMins);    
    Serial.print("textMins: ");
    Serial.println(textMins);
    
    hours = floor(mins / 60);
    char textHours[3];
    dtostrf(hours,1,0,textHours);
    Serial.print("textHours: ");
    Serial.println(textHours);
    
    // Concat char[]'s together since drawString only accepts one char[]
    char temp[10] = {""};
    (hours < leadingTenThreshold) ? strcat('0', textHours) : strcat(temp, textHours);
    strcat(temp, ':');
    (mins < leadingTenThreshold) ? strcat('0', textMins) : strcat(temp, textMins);
    strcat(temp, ':');
    (secs < leadingTenThreshold) ? strcat('0', textSecs) : strcat(temp, textSecs);
    Serial.println(temp);
    Serial.println("");
    
    // Increment the displayed timer by 1 second
    Tft.drawString(temp,140,180,2,WHITE);
    delay(900);
    Tft.drawString(temp,140,180,2,BLACK);
  }
}
