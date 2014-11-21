/* Tester for touchscreen */

#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>

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

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 140
#define TS_MAXX 900

#define TS_MINY 120
#define TS_MAXY 940


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Flag to keep track of current menu
int curMenu;

void setup(void) 
{
  Serial.begin(9600);
  Tft.init();  //init TFT library
  Tft.paintScreenBlack();  // Clear screen
  Tft.setDisplayDirect(UP2DOWN);

  // Draw welcome message
  //Tft.drawString("brewbot v.1", 50, 10, 3, WHITE);
  //delay(1000);

  // Set and draw main menu
  curMenu = 0;
  printMenu(curMenu);
  Serial.println("Main menu");
  Serial.print("curMenu = "); Serial.println(curMenu);
  Serial.println("");
}

void loop(void) 
{
  // a point object holds x y and z coordinates
  Point p = ts.getPoint();

  if (p.z > ts.pressureThreshhold)
  {
    p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
    Serial.print("x = "); Serial.print(p.x);
    Serial.print("\ty = "); Serial.print(p.y);
    Serial.print("\tz = "); Serial.println(p.z);

    // Check to see if a button was pressed
    if (curMenu == 0) // Main Menu
    {
      if (p.y >= 90 && p.y <= 230)
      {
        if (p.x >= 140 && p.x <= 200)
        {
          curMenu = 1; // H2O menu
          printMenu(curMenu);
          Serial.println("H2O");
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");

          // Used to 'clear out' the old p to prevent 'double clicking' 
          // on the button on the next generated screen. p gets reset to
          // a valid point at the top of the loop so the next touch can
          // be read. I couldn't find a way around using this 'hack'.
          p = ts.getPoint(); 
        }
        if (p.x >= 40 && p.x <= 100)
        {
          curMenu = 2; // Start menu
          printMenu(curMenu);
          Serial.println("Start");
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");
          p = ts.getPoint(); 
        }
      }
    }
    if (curMenu == 1) // H2O Menu
    {
      if (p.y >= 10 && p.y <= 246)
      {
        if (p.x >= 184 && p.x <= 224)
        {
          curMenu = 3;
          printMenu(curMenu);
          Serial.println("Pale Ale"); // Tft.fillRectangle(184, 10, 40, 236, BLUE);
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");
        }
        if (p.x >= 128 && p.x <= 168)
        {
          curMenu = 4;
          printMenu(curMenu);
          Serial.println("Amber/ESB"); // Tft.fillRectangle(128, 10, 40, 236, BLUE);
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");
        }
        if (p.x >= 72 && p.x <= 112)
        {
          curMenu = 5;
          printMenu(curMenu);
          Serial.println("Brown/Porter"); // Tft.fillRectangle(72, 10, 40, 236, BLUE);
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");
        }
        if (p.x >= 16 && p.x <= 56)
        {
          curMenu = 6;
          printMenu(curMenu);
          Serial.println("Stout"); // Tft.fillRectangle(16, 10, 40, 236, BLUE);
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");
        }
      }
      if (p.y >= 254 && p.y <= 314)
      {
        if (p.x >= 16 && p.x <= 224)
        {
          curMenu = 0;
          printMenu(curMenu);
          Serial.println("Back"); // Tft.fillRectangle(16, 254, 208, 60, RED);
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");
        }
      }
    }
    if (curMenu == 2 || curMenu == 3 || curMenu == 4 || curMenu == 5 || curMenu == 6)
    {
      if (p.y >= 254 && p.y <= 314)
      {
        if (p.x >= 16 && p.x <= 224)
        {
          curMenu = 1;
          printMenu(curMenu);
          Serial.println("Back");
          Serial.print("curMenu = "); Serial.println(curMenu);
          Serial.println("");
        }
      }
    }
  }
  delay(100);
}

void printMenu(int m)
{
  if (m == 0)
  {
    Tft.paintScreenBlack();
    Tft.fillRectangle(140, 90, 60, 140, BLUE); // {y,x,h,w,color}
    Tft.drawString("H2O", 180, 125, 3, WHITE); // {y,x,font-size,color}
    Tft.fillRectangle(40, 90, 60, 140, GREEN);
    Tft.drawString("Start", 80, 105, 3, WHITE);
  }
  if (m == 1)
  {
    Tft.paintScreenBlack();
    Tft.fillRectangle(184, 10, 40, 236, BLUE);
    Tft.drawString("Pale Ale", 204, 20, 2, WHITE); 
    
    Tft.fillRectangle(128, 10, 40, 236, BLUE);
    Tft.drawString("Amber/ESB", 148, 20, 2, WHITE);

    Tft.fillRectangle(72, 10, 40, 236, BLUE);
    Tft.drawString("Brown/Porter", 92, 20, 2, WHITE);

    Tft.fillRectangle(16, 10, 40, 236, BLUE);
    Tft.drawString("Stout", 36, 20, 2, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60, 274, 3, WHITE);
  }
  if (m == 2)
  {
    Tft.paintScreenBlack();
    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60, 274, 3, WHITE);
  }
  if (m == 3)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Pale Ale    5gal   10gal", 210, 10, 1, RED);
    Tft.drawString("Dilution:    50%     50%", 180, 10, 1, WHITE);
    Tft.drawString("Gypsum:     7.0g   14.0g", 150, 10, 1, WHITE);
    Tft.drawString("Salt:       1.2g    2.4g", 120, 10, 1, WHITE);
    Tft.drawString("CaCl2:      1.0g    2.0g", 90, 10, 1, WHITE);
    Tft.drawString("Pick Lime:  0.2g    0.4g", 60, 10, 1, WHITE);
    Tft.drawString("MgCl2:      1.0g    2.0g", 30, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
  if (m == 4)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Amber/ESB    5gal   10gal", 210, 10, 1, RED);
    Tft.drawString("Dilution:      0%      0%", 180, 10, 1, WHITE);
    Tft.drawString("Gypsum:     1.80g   3.60g", 150, 10, 1, WHITE);
    Tft.drawString("Salt:       0.00g   0.00g", 120, 10, 1, WHITE);
    Tft.drawString("CaCl2:      1.20g   2.40g", 90, 10, 1, WHITE);
    Tft.drawString("Phos Acid:  0.80g   1.60g", 60, 10, 1, WHITE);
    Tft.drawString("MgCl2:      1.12g   2.24g", 30, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
  if (m == 5)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Brwn/Prtr    5gal   10gal", 210, 10, 1, RED);
    Tft.drawString("Dilution:     50%     50%", 180, 10, 1, WHITE);
    Tft.drawString("Gypsum:     1.80g   3.60g", 150, 10, 1, WHITE);
    Tft.drawString("Pick Lime:  0.40g   0.80g", 120, 10, 1, WHITE);
    Tft.drawString("CaCl2:      0.80g   1.60g", 90, 10, 1, WHITE);
    Tft.drawString("Phos Acid:  0.80g   1.60g", 60, 10, 1, WHITE);
    Tft.drawString("MgCl2:      1.12g   2.24g", 30, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
  if (m == 6)
  {
    Tft.paintScreenBlack();
    Tft.drawString("Stout        5gal   10gal", 225, 10, 1, RED);
    Tft.drawString("Dilution:     25%     25%", 195, 10, 1, WHITE);
    Tft.drawString("Gypsum:      0.8g   1.6g", 165, 10, 1, WHITE);
    Tft.drawString("Salt:        0.4g   0.8g", 135, 10, 1, WHITE);
    Tft.drawString("CaCl2:       0.4g   0.8g", 105, 10, 1, WHITE);
    Tft.drawString("Phos Acid:   1.0g   2.0g", 75, 10, 1, WHITE);
    Tft.drawString("Pick Lime:   0.8g   1.6g", 45, 10, 1, WHITE);
    Tft.drawString("MgCl2:       1.0g   2.0g", 15, 10, 1, WHITE);

    Tft.fillRectangle(16, 254, 208, 60, RED);
    Tft.drawString("B", 180, 274, 3, WHITE);
    Tft.drawString("A", 140, 274, 3, WHITE);
    Tft.drawString("C", 100, 274, 3, WHITE);
    Tft.drawString("K", 60,  274, 3, WHITE);
  }
}
