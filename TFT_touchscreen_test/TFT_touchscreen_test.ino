/**
 * Seeedstudio LCD 2.8" TFT touchscreen
 * Will display data from two temp sensors and has a countdown clock.
 * Intended use if for the brew_bot.v2
 */
#include <stdint.h>
//#include <TouchScreen.h> // Will be used to read touch events
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
unsigned long currentMillis = 0;  // current millis
unsigned long previousMillis = 0;  // Will store last time Timer was updated
unsigned long benchMillis = 0;  // Variable used to be able to reset second to zero and still be able to keep track of time
int second = 0, minute = 0, hour = 0; // Keep track of time so we can increment min/hours based on elapsed seconds
const long MILLIS_IN_MINUTE = 60000;  // Constant to calculate printable time
char _hour[3]; // Buffer to store the current hour in. Used to compare to see if it has changed and needs to be updated. Prevents blinking every pass through main loop.
char _minute[3];
char _second[3];

void setup()
{
  Serial.begin(9600);
  Tft.init();  //init TFT library
  Tft.paintScreenBlack();  // Clear screen

  Tft.setDisplayDirect(UP2DOWN);
  Tft.drawString("Upper: ",220,20,2,WHITE);
  Tft.drawString("Mash: ",180,20,2,WHITE);
  Tft.drawString("Timer: ",140,20,2,WHITE);
  Tft.fillRectangle(10, 20, 50, 125, GREEN);
  Tft.fillRectangle(10, 175, 50, 125, RED);
  Tft.drawString("Start",45,27,3,BLACK);
  Tft.drawString("Stop",45,193,3,BLACK);
  previousMillis = millis();
}

void loop()
{

  /***** READ SENSORS AND DISPLAY *****/
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
  /***** END READ SENSORS AND DISPLAY *****/

  /***** TIMER *****/
  // Check to see if 1 second has elapsed
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
    // Save the last time you updated Timer
    previousMillis = currentMillis;
    
    /*** UPDATE TIMER COMPONENTS ***/
    if (currentMillis > benchMillis + MILLIS_IN_MINUTE) 
    {
      minute++;
      second = 0;
      benchMillis = currentMillis;
    }
    else
    {
      second = (currentMillis - benchMillis) / 1000;
    }

    if (minute > 59) 
    {
      hour++;
      minute = 0;
    }
    
    // Update the timer on the display
    updateDisplayTimer();
  }/***** END TIMER *****/
}/***** END MAIN LOOP *****/

/***** USER DEFINED FUNCTIONS *****/
void updateDisplayTimer()
{
  sprintf(_hour, "%02d", hour); // Arguments are: (buffer, format, value to print)
  Serial.print(_hour);
  Serial.print(":");
  
  sprintf(_minute, "%02d", minute); 
  Serial.print(_minute);
  Serial.print(":");  
  
  sprintf(_second, "%02d", second);
  Serial.println(_second);
  Serial.println("");
  
  /*** END UPDATE TIMER COMPONENTS ***/
  
  /*** DISPLAY TEXT ***/
  Tft.drawString(_hour,140,180,2,WHITE);
  Tft.drawString(":",140,210,2,WHITE); // 30px spacing since its double digits
  
  Tft.drawString(_minute,140,225,2,WHITE);
  Tft.drawString(":",140,255,2,WHITE);
  
  Tft.drawString(_second,140,270,2,WHITE);
  /*** END DISPLAY TEXT ***/
  
  /*** ERASE TEXT ***/
  Tft.drawString(_hour,140,180,2,BLACK);
  Tft.drawString(_minute,140,225,2,BLACK);
  Tft.drawString(_second,140,270,2,BLACK);
  /*** ERASE TEXT ***/
}

