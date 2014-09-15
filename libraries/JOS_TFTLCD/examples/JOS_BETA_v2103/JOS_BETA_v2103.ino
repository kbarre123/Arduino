/* 
   JOS TFT Touch Beta   version v2.1.0.3 3/07/2013
   Adafruit_TFTLCD.h and Adafruit_TouchScreen.h are from adafruit.com where you can also purchase a really nice 2.8" TFT with touchscreen :)
   Oringinal Sketch written by jeremy saglimbeni: www.thecustomgeek.com 
   Updates for uno/mega nathan sobieck: www.sobisource.com 
       backlight = pin 10 uno / pin 35 mega
       Must use libraries included / which are Adafruit's current Adafruit_GFX, Adafruit_TFTLCD and TouchScreen Libraries
       as of 3/07/13 
	   
	   *Note if you are using Adafruits TFTLCD Shield. Then you need to uncomment #define USE_ADAFRUIT_SHIELD_PINOUT 
	   in Adafruit_TFTLCD > Adafruit_TFTLCD.h  
       However if you are using the shield Backlight PWM will not function unless you make a few simple hardware changes...
	   Learn How Here....http://learn.adafruit.com/2-8-tft-touch-shield/controlling-the-backlight
	   
*/
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <EEPROM.h>
/* For the 8 data pins:
 Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontoller:
 D0 connects to digital 8
 D1 connects to digital 9
 D2 connects to digital 2
 D3 connects to digital 3
 D4 connects to digital 4
 D5 connects to digital 5
 D6 connects to digital 6
 D7 connects to digital 7
 For Mega's use pins 22 thru 29 (on the double header at the end)
 */

#define YP A3  // must be an analog pin, use "An" notation!  A1 for shield
#define XM A2  // must be an analog pin, use "An" notation!  A2 for shield
#define YM 9   // can be a digital pin-----uno=9 mega=23      7 for shield
#define XP 8   // can be a digital pin-----uno=8 mega=22      6 for shield
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0 
// optional
#define LCD_RESET A4
// Color definitions - in 5:6:5
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF
#define TEST            0x1BF5
#define JJCOLOR         0x1CB6
#define JJORNG          0xFD03

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
int i = 0;
int page = 0;
int blv;
int sleep = 0;
int pulsev = 0;
int redflag = 0;
int greenflag = 0;
int redled = 12;
int greenled = 13;
int backlight = 10;  //pin-10 uno  pin-35 mega  pin-3 for shield if you have done the MOD   for PWM backlight
int battfill;
unsigned long sleeptime;
unsigned long battcheck = 10000; // the amount of time between voltage check and battery icon refresh
unsigned long prevbatt;

int battv;
int battold;
int battpercent;
int barv;
int prevpage;
int sleepnever;
int esleep;
int backlightbox;
int antpos = 278;
unsigned long awakeend;
unsigned long currenttime;
unsigned long ssitime;
char voltage[10];
char battpercenttxt [10];


int getBandgap(void) // Returns actual value of Vcc (x 100) 415 = 4.15v
{
        
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
     // For mega boards
     const long InternalReferenceVoltage = 1115L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc reference
        // MUX4 MUX3 MUX2 MUX1 MUX0  --> 11110 1.1V (VBG)         -Selects channel 30, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR)| (0<<MUX5) | (1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
  
#else
     // For 168/328 boards
     const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
        // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
       
#endif
     delay(50);  // Let mux settle a little to get a more stable A/D conversion
        // Start a conversion  
     ADCSRA |= _BV( ADSC );
        // Wait for it to complete
     while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
        // Scale the value
     int results = (((InternalReferenceVoltage * 1023L) / ADC) + 5L) / 10L; // calculates for straight line value 
     return results;
     }

void setup(void) {
  
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  analogReference(5);
  esleep = EEPROM.read(1);
  blv = EEPROM.read(2);
  if (esleep == 0 && blv == 0) {  // if no previous saves then set sleep and backlight to default. 
   EEPROM.write(1, 4);
   EEPROM.write(2, 200); 
  }
  if (esleep == 1) {
    sleeptime = 10000;
  }
  if (esleep == 2) {
    sleeptime = 20000;
  }
  if (esleep == 3) {
    sleeptime = 30000;
  }
  if (esleep == 4) {
    sleeptime = 60000;
  }
  if (esleep == 5) {
    sleeptime = 120000;
  }
  if (esleep == 6) {
    sleeptime = 300000;
  }
  if (esleep == 7) {
    sleeptime = 600000;
  }
  if (esleep == 8) {
    sleeptime = 1200000;
  }
  if (esleep == 9) {
    sleeptime = 1800000;
  }
  if (esleep == 10) {
    sleeptime = 3600000;
  }
  if (esleep == 11) {
    sleeptime = 14400000;
  }
  if (esleep == 12) {
    sleepnever = 1;
  }
  awakeend = sleeptime + 30000; // set the current sleep time based on what the saved settings in EEPROM were
  pinMode(backlight, OUTPUT);
  Serial.begin(9600);
  Serial.println("JOS");
  Serial.println("Jeremy Saglimbeni (c) 2011");
  
  tft.reset();
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-==--=-=-=-=-=-=-=
  // All this idenifies which LCD driver chip your using and tells Ada's GFX LIB's 
  uint16_t identifier = tft.readID();  

  if(identifier == 0x9325) {
    progmemPrintln(PSTR("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    progmemPrintln(PSTR("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    progmemPrintln(PSTR("Found HX8347G LCD driver"));
  } else {
    progmemPrint(PSTR("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(identifier);
    return;
  }
  tft.begin(identifier);  // this returns the above results. below you can see how I replaced all this once i know which one I have... 
  //tft.begin(0x9328); //<---Here I bypassed the above. cause I already checked...
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-
  
  tft.setRotation(3); //sets landscape mode
  tft.fillScreen(BLACK);
  tft.fillRect(71, 70, 50, 100, JJCOLOR);
  tft.fillRect(134, 70, 50, 100, JJCOLOR);
  tft.fillRect(197, 70, 50, 100, JJCOLOR);
  tft.drawRect(46, 45, 228, 150, WHITE);
  for(i = 0 ; i <= blv; i+=1) { 
    analogWrite(backlight, i);
    delay(2);
  }
  delay(250);
  tft.setTextSize(5);
  tft.setTextColor(WHITE);
  tft.setCursor(85, 100);
  tft.println("J");
  delay(250);
  tft.setCursor(147, 100);
  tft.println("O");
  delay(250);
  tft.setCursor(210, 100);
  tft.println("S");
  delay(500);
  tft.setTextSize(1);
  tft.setCursor(100, 210);
  tft.println("Jeremy Saglimbeni 2013");
  tft.setCursor(120, 230);
  tft.println("TheCustomGeek.com");
  delay(2000);//introscreen delay
  tft.fillScreen(BLACK);
  tft.fillRect(0, 0, 320, 10, JJCOLOR); // status bar
  drawhomeicon(); // draw the home icon
  tft.setCursor(1, 1);
  tft.println("Your status bar message here.    JOS 2.1 Beta");
  tft.drawRect(297, 1, 20, 8, WHITE); //battery body
  tft.fillRect(317, 3, 2, 4, WHITE); // battery tip
  tft.fillRect(298, 2, 18, 6, BLACK); // clear the center of the battery
  drawbatt();
  ant(); // draw the bas "antenna" line without the "signal waves"
  signal(); // draw the "signal waves" around the "antenna"
  homescr(); // draw the homescreen
  tft.drawRect(0, 200, 245, 40, WHITE); // message box
}
#define MINPRESSURE 10
#define MAXPRESSURE 1000
void loop() {
  
  
  currenttime = millis();
  unsigned long currentawake = millis();
  if((currentawake > awakeend) && (sleepnever == 0)) {
    if (sleep == 0) {
      for(i = blv ; i >= 0; i-=1) { 
        analogWrite(backlight, i);
        delay(4);
      }
      sleep = 1;
    }
  }
  Point p = ts.getPoint();
  // if you're sharing pins, you'll need to fix the directions of the touchscreen pins!
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    awakeend = currenttime + sleeptime; //set the sleep time when screen is pressed
    if (sleep == 1) { // if asleep, then fade the backlight up
      for(i = 0 ; i <= blv; i+=1) { 
        analogWrite(backlight, i);
        delay(1);
      }
      sleep = 0; // change the sleep mode to "awake"
      return;
    }
   
    // turn from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);//default is (240, 0) [default puts touch cord. 0=x/y upper right.
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);//default is (320, 0) [I change these cause i like 0=xy bottom left.
   
    Serial.print("p.y:"); // this code will help you get the y and x numbers for the touchscreen
    Serial.print(p.y);
    Serial.print("   p.x:");
    Serial.println(p.x);
     
    // area 1
    if (p.y > 1 && p.y < 141 && p.x > 175 && p.x < 222) { // if this area is pressed
      if (page == 5) { // and if page 5 is drawn on the screen
        m5b1action(); // do whatever this button is
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 5 B1"); // display the command in the "message box"
        yled(550); // flash the LED yellow for a bit - change the 550 value to change LED time on
        clearmessage(); // after the LED goes out, clear the message
      }
      if (page == 4) {
        m4b1action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 4 B1");
        yled(550);
        clearmessage();
      }
      if (page == 3) {
        m3b1action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 3 B1");
        yled(550);
        clearmessage();
      }
      if (page == 2) {
        m2b1action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 2 B1");
        yled(550);
        clearmessage();
      }
      if (page == 1) {
        m1b1action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 1 B1");
        yled(550);
        clearmessage();
      }
      if (page == 0) { // if you are on the "home" page (0)
        page = 1; // then you just went to the first page
        redraw(); // redraw the screen with the page value 1, giving you the page 1 menu
      }
    }
    // area 2
    if (p.y > 165 && p.y < 315 && p.x > 176 && p.x < 222) {
      if (page == 5) {
        m5b2action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 5 B2");
        yled(550);
        clearmessage();
      }
      if (page == 4) {
        m4b2action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 4 B2");
        yled(550);
        clearmessage();
      }
      if (page == 3) {
        m3b2action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 3 B2");
        yled(550);
        clearmessage();
      }
      if (page == 2) {
        m2b2action();tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 2 B2");
        yled(550);
        clearmessage();
      }
      if (page == 1) {
        m1b2action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 1 B2");
        yled(550);
        clearmessage();
      }
      if (page == 0) {
        page = 2;
        redraw();
      }
    }
    // area 3
    if (p.y > 1 && p.y < 141 && p.x > 115 && p.x < 163) {
      if (page == 5) {
        m5b3action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 5 B3");
        yled(550);
        clearmessage();
      }
      if (page == 4) {
        m4b3action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 4 B3");
        yled(550);
        clearmessage();
      }
      if (page == 3) {
        m3b3action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 3 B3");
        yled(550);
        clearmessage();
      }
      if (page == 2) {
        m2b3action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 2 B3");
        yled(550);
        clearmessage();
      }
      if (page == 1) {
        m1b3action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 1 B3");
        yled(550);
        clearmessage();
      }
      if (page == 0) {
        page = 3;
        redraw();
      }
    }
    // area 4
    if (p.y > 165 && p.y < 315 && p.x > 115 && p.x < 163) {
      if (page == 5) {
        m5b4action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 5 B4");
        yled(550);
        clearmessage();
      }
      if (page == 4) {
        m4b4action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 4 B4");
        yled(550);
        clearmessage();
      }
      if (page == 3) {
        m3b4action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 3 B4");
        yled(550);
        clearmessage();
      }
      if (page == 2) {
        m2b4action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 2 B4");
        yled(550);
        clearmessage();
      }
      if (page == 1) {
        m1b4action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 1 B4");
        yled(550);
        clearmessage();
      }
      if (page == 0) {
        page = 4;
        redraw();
      }
    }
    // area 5
    if (p.y > 1 && p.y < 141 && p.x > 57 && p.x < 101) {
      if (page == 5) {
        m5b5action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 5 B5");
        yled(550);
        clearmessage();
      }
      if (page == 4) {
        m4b5action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 4 B5");
        yled(550);
        clearmessage();
      }
      if (page == 3) {
        m3b5action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 3 B5");
        yled(550);
        clearmessage();
      }
      if (page == 2) {
        m2b5action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 2 B5");
        yled(550);
        clearmessage();
      }
      if (page == 1) {
        m1b5action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 1 B5");
        yled(550);
        clearmessage();
      }
      if (page == 0) {
        page = 5;
        redraw();
      }
    }
    // area 6
    if (p.y > 165 && p.y < 315 && p.x > 57 && p.x < 101) {
      if (page == 5) {
        m5b6action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 5 B6");
        yled(550);
        clearmessage();
      }
      if (page == 4) {
        m4b6action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 4 B6");
        yled(550);
        clearmessage();
      }
      if (page == 3) {
        m3b6action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 3 B6");
        yled(550);
        clearmessage();
      }
      if (page == 2) {
        m2b6action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 2 B6");
        yled(550);
        clearmessage();
      }
      if (page == 1) {
        m1b6action();
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Menu 1 B6");
        yled(550);
        clearmessage();
      }
      if (page == 0) {
        page = 6;
        redraw();
      }
    }
    // home
    if (p.y > 269 && p.y < 320 && p.x > 1 && p.x < 47) { // if the home icon is pressed
      if (page == 6) { // if you are leaving the settings page
        clearmessage(); // clear the battery voltage out of the message box
        tft.setCursor(12, 213);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Settings Saved"); // display settings saved in message box
        EEPROM.write(1, esleep); // write the sleep value to EEPROM, so it will not lose settings without power
        EEPROM.write(2, blv); // write the backlight value to EEPROM, so it will not lose settings without power
        clearsettings(); // erase all the drawings on the settings page
      }
      if (page == 0) { // if you are already on the home page
        drawhomeiconred(); // draw the home icon red
        delay(250); // wait a bit
        drawhomeicon(); // draw the home icon back to white
        return; // if you were on the home page, stop.
      }
      else { // if you are not on the settings, home, or keyboard page
        page = prevpage; // a value to keep track of what WAS on the screen to redraw/erase only what needs to be
        page = 0; // make the current page home
        redraw(); // redraw the page
      }
    }
    // message area
    if (p.y > 1 && p.y < 240 && p.x > 1 && p.x < 44) {
      clearmessage(); // erase the message
    }
    // backlight buttons
    if (p.y > 1 && p.y < 50 && p.x > 175 && p.x < 222) {  //-
      if (page == 6) {
        blightdown();
      }
    }
    if (p.y > 259 && p.y < 315 && p.x > 175 && p.x < 222) {  //+
      if (page == 6) {
        blightup();
      }
    }
    // sleep buttons
    if (p.y > 1 && p.y < 50 && p.x > 115 && p.x < 163) {
      if (page == 6) {
        sleepdec();
      }
    }
    if (p.y > 259 && p.y < 315 && p.x > 115 && p.x < 163) {
      if (page == 6) {
        sleepinc();
      }
    }
    /*
    // optional buttons
     if (p.y > 1 && p.y < 50 && p.x > 52 && p.x < 100) {
     if (page == 6) {
     option3down();
     }
     }
     if (p.y > 259 && p.y < 315 && p.x > 52 && p.x < 100) {
     if (page == 6) {
     option3up();
     }
     }
     */
  }
  if(currenttime - prevbatt > battcheck) {
    drawbatt();
    prevbatt = currenttime;

  }
}
void yled(int xled) { // "flashes" the "yellow" LED
  for(i = xled ; i >= 0; i-=1) { 
    digitalWrite(greenled, LOW);
    digitalWrite(redled, HIGH);
    delay(1);
    digitalWrite(greenled, HIGH);
    digitalWrite(redled, LOW);
    delay(1);    
  }
  digitalWrite(greenled, LOW);
  if (greenflag == 1) {
    digitalWrite(redled, LOW);
    digitalWrite(greenled, HIGH);
  }
  if (redflag == 1) {
    digitalWrite(greenled, LOW);
    digitalWrite(redled, HIGH);
  }
}
void redraw() { // redraw the page
  if ((prevpage != 6) || (page !=7)) {
    clearcenter();
  }
  if (page == 0) {
    homescr();
  }
  if (page == 1) {
    menu1();
  }
  if (page == 2) {
    menu2();
  }
  if (page == 3) {
    menu3();
  }
  if (page == 4) {
    menu4();
  }
  if (page == 5) {
    menu5();
  }
  if (page == 6) {
    settingsscr();
  }
}
void clearcenter() { // the reason for so many small "boxes" is that it's faster than filling the whole thing
  tft.drawRect(0, 20, 150, 50, BLACK);
  tft.drawRect(170, 20, 150, 50, BLACK);
  tft.drawRect(0, 80, 150, 50, BLACK);
  tft.drawRect(170, 80, 150, 50, BLACK);
  tft.drawRect(0, 140, 150, 50, BLACK);
  tft.drawRect(170, 140, 150, 50, BLACK);
  tft.fillRect(22, 37, 106, 16, BLACK);
  tft.fillRect(192, 37, 106, 16, BLACK);
  tft.fillRect(22, 97, 106, 16, BLACK);
  tft.fillRect(192, 97, 106, 16, BLACK);
  tft.fillRect(22, 157, 106, 16, BLACK);
  tft.fillRect(192, 157, 106, 16, BLACK);
}
void clearsettings() { // this is used to erase the extra drawings when exiting the settings page
  tft.fillRect(0, 20, 320, 110, BLACK);
  delay(500);
  clearmessage();
}
void homescr() {
  boxes();
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
  tft.setCursor(41, 37);
  tft.println("Menu 1");
  tft.setCursor(210, 37);
  tft.println("Menu 2");
  tft.setCursor(41, 97);
  tft.println("Menu 3");
  tft.setCursor(210, 97);
  tft.println("Menu 4");
  tft.setCursor(41, 157);
  tft.println("Menu 5");
  tft.setCursor(200, 157);
  tft.println("Settings");
}
void menu1() {
  boxes();
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
  tft.setCursor(22, 37);
  tft.println("Menu 1 B1");
  tft.setCursor(192, 37);
  tft.println("Menu 1 B2");
  tft.setCursor(22, 97);
  tft.println("Menu 1 B3");
  tft.setCursor(192, 97);
  tft.println("Menu 1 B4");
  tft.setCursor(22, 157);
  tft.println("Menu 1 B5");
  tft.setCursor(192, 157);
  tft.println("Menu 1 B6");
}
void menu2() {
  boxes();
        tft.setTextColor(WHITE);
        tft.setTextSize(2);
  tft.setCursor(22, 37);
  tft.println("Menu 2 B1");
  tft.setCursor(192, 37);
  tft.println("Menu 2 B2");
  tft.setCursor(22, 97);
  tft.println("Menu 2 B3");
  tft.setCursor(192, 97);
  tft.println("Menu 2 B4");
  tft.setCursor(22, 157);
  tft.println("Menu 2 B5");
  tft.setCursor(192, 157);
  tft.println("Menu 2 B6");
}
void menu3() {
  boxes();
  tft.setTextColor(WHITE);
        tft.setTextSize(2);
  tft.setCursor(22, 37);
  tft.println("Menu 3 B1");
  tft.setCursor(192, 37);
  tft.println("Menu 3 B2");
  tft.setCursor(22, 97);
  tft.println("Menu 3 B3");
  tft.setCursor(192, 97);
  tft.println("Menu 3 B4");
  tft.setCursor(22, 157);
  tft.println("Menu 3 B5");
  tft.setCursor(192, 157);
  tft.println("Menu 3 B6");
}
void menu4() {
  boxes();
  tft.setTextColor(WHITE);
        tft.setTextSize(2);
  tft.setCursor(22, 37);
  tft.println("Menu 4 B1");
  tft.setCursor(192, 37);
  tft.println("Menu 4 B2");
  tft.setCursor(22, 97);
  tft.println("Menu 4 B3");
  tft.setCursor(192, 97);
  tft.println("Menu 4 B4");
  tft.setCursor(22, 157);
  tft.println("Menu 4 B5");
  tft.setCursor(192, 157);
  tft.println("Menu 4 B6");
}
void menu5() {
  boxes();
  tft.setTextColor(WHITE);
        tft.setTextSize(2);
  tft.setCursor(22, 37);
  tft.println("Menu 5 B1");
  tft.setCursor(192, 37);
  tft.println("Menu 5 B2");
  tft.setCursor(22, 97);
  tft.println("Menu 5 B3");
  tft.setCursor(192, 97);
  tft.println("Menu 5 B4");
  tft.setCursor(22, 157);
  tft.println("Menu 5 B5");
  tft.setCursor(192, 157);
  tft.println("Menu 5 B6");
}
void settingsscr() {
  
  // backlight level
  tft.fillRect(0, 20, 60, 50, RED);
  tft.drawRect(0, 20, 60, 50, WHITE);
  tft.drawRect(80, 20, 160, 50, JJCOLOR);
  tft.fillRect(260, 20, 60, 50, GREEN);
  tft.drawRect(260, 20, 60, 50, WHITE);
        tft.setTextColor(WHITE);
        tft.setTextSize(3);
        tft.setCursor(22, 33);
  tft.println("-");
        tft.setTextSize(1);
        tft.setCursor(120, 31);
  tft.println("Backlight Level");
        tft.setTextSize(3);
        tft.setCursor(282, 33);
  tft.println("+");
  tft.drawRect(110, 48, 100, 10, WHITE);
  blbar();
  // sleep time
  tft.fillRect(0, 80, 60, 50, RED);
  tft.drawRect(0, 80, 60, 50, WHITE);
  tft.drawRect(80, 80, 160, 50, JJCOLOR);
  tft.fillRect(260, 80, 60, 50, GREEN);
  tft.drawRect(260, 80, 60, 50, WHITE);
         tft.setTextSize(3);
         tft.setCursor(22, 93);
  tft.println("-");
         tft.setTextSize(2);
         tft.setCursor(100, 91);
  tft.println("Sleep Time");
         tft.setTextSize(3);
         tft.setCursor(282, 93);
  tft.println("+");
  showsleep();
  //?? uncomment this if you want a third adjustable option
  /*
  tft.fillRect(0, 140, 60, 50, RED);
   tft.drawRect(0, 140, 60, 50, WHITE);
   tft.drawRect(80, 140, 160, 50, JJCOLOR);
   tft.fillRect(260, 140, 60, 50, GREEN);
   tft.drawRect(260, 140, 60, 50, WHITE);
          tft.setTextSize(3);
          tft.setCursor(22, 153);
   tft.println("-");
         tft.setTextSize(1);
         tft.setCursor(130, 151);
   tft.println("Thing #3");
         tft.setTextSize(3);
         tft.setCursor(282, 153);
   tft.drawString("+");
   tft.drawRect(110, 168, 100, 10, WHITE);
   */
  
 
  battv = getBandgap(); // read the voltage getBandgap
  itoa (battv, voltage, 10); // converts integer to string
  decimate(voltage, 2); // converts string to decimal point
         tft.setTextColor(YELLOW);
         tft.setTextSize(2);
         tft.setCursor(25, 213);
  tft.println(voltage);
         tft.setTextColor(YELLOW);
         tft.setTextSize(2);
         tft.setCursor(13, 213);
  tft.println("V");
  
  battpercent = (battv / 480) * 100, 2;  // if using battery set 480(4.80v) to fresh charge value
  battpercent = constrain(battpercent, 1, 100);  // battery percent between 1% and 100%
  itoa (battpercent, battpercenttxt, 10);
         tft.setTextColor(YELLOW);
         tft.setTextSize(2);
         tft.setCursor(130, 213);
  tft.println(battpercenttxt);
         tft.setTextColor(YELLOW);
         tft.setTextSize(2);
         tft.setCursor(115, 213);
  tft.println("%");
}

void decimate(char test[],int dec) {  // creates decimal function  decimate(string containing integer, number of decimal places)

 int i=0;  
 int length=strlen(test);
 char msg[10]="";

 strcpy(msg,test);

 if (length <= dec) {
   for(i=dec;i>(dec-length);i--)  msg[i] = msg[i-(dec-length+1)];
   for(i=0;i<(dec+1-length);i++)  msg[i]='0';
   length = strlen(msg);
 }
 for (i=length;i>(length-dec);i--)  msg[i]=msg[i-1];
 msg[length-dec]='.';

 strcpy(test,msg);
} 

void sleepinc() { // sleep increese adjustment
  if (sleeptime == 14400000) {
    sleepnever = 1;
    esleep = 12;
    sleeptime = 11111111;
    showsleep();
  }
  if (sleeptime == 3600000) {
    sleeptime = 14400000;
    esleep = 11;
    showsleep();
  }
  if (sleeptime == 1800000) {
    sleeptime = 3600000;
    esleep = 10;
    showsleep();
  }
  if (sleeptime == 1200000) {
    sleeptime = 1800000;
    esleep = 9;
    showsleep();
  }
  if (sleeptime == 600000) {
    sleeptime = 1200000;
    esleep = 8;
    showsleep();
  }
  if (sleeptime == 300000) {
    sleeptime = 600000;
    esleep = 7;
    showsleep();
  }
  if (sleeptime == 120000) {
    sleeptime = 300000;
    esleep = 6;
    showsleep();
  }
  if (sleeptime == 60000) {
    sleeptime = 120000;
    esleep = 5;
    showsleep();
  }
  if (sleeptime == 30000) {
    sleeptime = 60000;
    esleep = 4;
    showsleep();
  }
  if (sleeptime == 20000) {
    sleeptime = 30000;
    esleep = 3;
    showsleep();
  }
  if (sleeptime == 10000) {
    sleeptime = 20000;
    esleep = 2;
    showsleep();
  }
  delay(350);
}
void sleepdec() { // sleep decreese adjustment
  if (sleeptime == 20000) {
    sleeptime = 10000;
    esleep = 1;
    showsleep();
  }
  if (sleeptime == 30000) {
    sleeptime = 20000;
    esleep = 2;
    showsleep();
  }
  if (sleeptime == 60000) {
    sleeptime = 30000;
    esleep = 3;
    showsleep();
  }
  if (sleeptime == 120000) {
    sleeptime = 60000;
    esleep = 4;
    showsleep();
  }
  if (sleeptime == 300000) {
    sleeptime = 120000;
    esleep = 5;
    showsleep();
  }
  if (sleeptime == 600000) {
    sleeptime = 300000;
    esleep = 6;
    showsleep();
  }
  if (sleeptime == 1200000) {
    sleeptime = 600000;
    esleep = 7;
    showsleep();
  }
  if (sleeptime == 1800000) {
    sleeptime = 1200000;
    esleep = 8;
    showsleep();
  }
  if (sleeptime == 3600000) {
    sleeptime = 1800000;
    esleep = 9;
    showsleep();
  }
  if (sleeptime == 14400000) {
    sleeptime = 3600000;
    esleep = 10;
    showsleep();
  }
  if (sleepnever == 1) {
    sleeptime = 14400000;
    sleepnever = 0;
    esleep = 11;
    showsleep();
  }
  delay(350);
}
void showsleep() { // shows the sleep time on the settings page
  tft.fillRect(110, 108, 80, 10, BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  if (sleeptime == 10000) {
    tft.setCursor(130, 108);
    tft.println("10 Seconds");
  }
  if (sleeptime == 20000) {
    tft.setCursor(130, 108);
    tft.println("20 Seconds");
  }
  if (sleeptime == 30000) {
    tft.setCursor(130, 108);
    tft.println("30 Seconds");
  }
  if (sleeptime == 60000) {
    tft.setCursor(136, 108);
    tft.println("1 Minute");
  }
  if (sleeptime == 120000) {
    tft.setCursor(133, 108);
    tft.println("2 Minutes");
  }
  if (sleeptime == 300000) {
    tft.setCursor(133, 108);
    tft.println("5 Minutes");
  }
  if (sleeptime == 600000) {
    tft.setCursor(130, 108);
    tft.println("10 Minutes");
  }
  if (sleeptime == 1200000) {
    tft.setCursor(130, 108);
    tft.println("20 Minutes");
  }
  if (sleeptime == 1800000) {
    tft.setCursor(130, 108);
    tft.println("30 Minutes");
  }
  if (sleeptime == 3600000) {
    tft.setCursor(142, 108);
    tft.println("1 Hour");
  }
  if (sleeptime == 14400000) {
    tft.setCursor(139, 108);
    tft.println("4 Hours");
  }
  if (sleepnever == 1) {
    tft.setCursor(133, 108);
    tft.println("Always On");
  }
}
void option3down() { // adjust option 3 down in the settings screen
}
void option3up() { // adjust option 3 up in the settings screen
}
//custom defined actions - this is where you put your button functions
void m1b1action() {
  signal();
}
void m1b2action() {
  signalact();
}
void m1b3action() {
}
void m1b4action() {
}
void m1b5action() {
}
void m1b6action() {
}
void m2b1action() {
}
void m2b2action() {
}
void m2b3action() {
}
void m2b4action() {
}
void m2b5action() {
}
void m2b6action() {
}
void m3b1action() {
}
void m3b2action() {
}
void m3b3action() {
}
void m3b4action() {
}
void m3b5action() {
}
void m3b6action() {
}
void m4b1action() {
}
void m4b2action() {
}
void m4b3action() {
}
void m4b4action() {
}
void m4b5action() {
}
void m4b6action() {
}
void m5b1action() {
}
void m5b2action() {
}
void m5b3action() {
}
void m5b4action() {
}
void m5b5action() {
}
void m5b6action() {
}
void blightup() { // increase the backlight brightness
  blv = blv + 5;
  if (blv >= 255) {
    blv = 255;
  }
  analogWrite(10, blv);
  blbar();
}
void blightdown() { // decrease the backlight brightness
  blv = blv - 5;
  if (blv <= 5) {
    blv = 5;
  }
  analogWrite(10, blv);
  blbar();
}
void blbar() { // this function fills the yellow bar in the backlight brightness adjustment
  if (blv < barv) {
    tft.fillRect(111, 49, 98, 8, BLACK);
  }
  backlightbox = map(blv, 1, 255, 0, 98);
  tft.fillRect(111, 49, backlightbox, 8, YELLOW);
  barv = blv;
  delay(25);
}
void ant() {
  tft.fillRect((antpos + 5), 4, 1, 6, WHITE); // draws the "antenna" for the signal indicator
}
void boxes() { // redraw the button outline boxes
  tft.drawRect(0, 20, 150, 50, JJCOLOR);
  tft.drawRect(170, 20, 150, 50, JJCOLOR);
  tft.drawRect(0, 80, 150, 50, JJCOLOR);
  tft.drawRect(170, 80, 150, 50, JJCOLOR);
  tft.drawRect(0, 140, 150, 50, JJCOLOR);
  tft.drawRect(170, 140, 150, 50, JJCOLOR);
}
void signal() { // draws a whit 'signal indicator'
  tft.drawLine((antpos + 4), 4, (antpos + 4), 5, WHITE);
  tft.drawPixel((antpos + 3), 2, WHITE);
  tft.drawPixel((antpos + 3), 7, WHITE);
  tft.drawPixel((antpos + 2), 0, WHITE);
  tft.drawLine((antpos + 2), 3, (antpos + 2), 6, WHITE);
  tft.drawPixel((antpos + 2), 9, WHITE);
  tft.drawPixel((antpos + 1), 1, WHITE);
  tft.drawPixel((antpos + 1), 8, WHITE);
  tft.drawLine(antpos, 2, antpos, 7, WHITE);
  tft.drawLine((antpos + 6), 4, (antpos + 6), 5, WHITE);
  tft.drawPixel((antpos + 7), 2, WHITE);
  tft.drawPixel((antpos + 7), 7, WHITE);
  tft.drawPixel((antpos + 8), 0, WHITE);
  tft.drawLine((antpos + 8), 3, (antpos + 8), 6, WHITE);
  tft.drawPixel((antpos + 8), 9, WHITE);
  tft.drawPixel((antpos + 9), 1, WHITE);
  tft.drawPixel((antpos + 9), 8, WHITE);
  tft.drawLine((antpos + 10), 2, (antpos + 10), 7, WHITE);
}
void signalact() { // draws a red'signal indicator'
  tft.drawLine((antpos + 4), 4, (antpos + 4), 5, RED);
  tft.drawPixel((antpos + 3), 2, RED);
  tft.drawPixel((antpos + 3), 7, RED);
  tft.drawPixel((antpos + 2), 0, RED);
  tft.drawLine((antpos + 2), 3, (antpos + 2), 6, RED);
  tft.drawPixel((antpos + 2), 9, RED);
  tft.drawPixel((antpos + 1), 1, RED);
  tft.drawPixel((antpos + 1), 8, RED);
  tft.drawLine(antpos, 2, antpos, 7, RED);
  tft.drawLine((antpos + 6), 4, (antpos + 6), 5, RED);
  tft.drawPixel((antpos + 7), 2, RED);
  tft.drawPixel((antpos + 7), 7, RED);
  tft.drawPixel((antpos + 8), 0, RED);
  tft.drawLine((antpos + 8), 3, (antpos + 8), 6, RED);
  tft.drawPixel((antpos + 8), 9, RED);
  tft.drawPixel((antpos + 9), 1, RED);
  tft.drawPixel((antpos + 9), 8, RED);
  tft.drawLine((antpos + 10), 2, (antpos + 10), 7, RED);
}
void drawhomeicon() { // draws a white home icon
  tft.drawLine(280, 219, 299, 200, WHITE);
  tft.drawLine(300, 200, 304, 204, WHITE);
  tft.drawLine(304, 203, 304, 200, WHITE);
  tft.drawLine(305, 200, 307, 200, WHITE);
  tft.drawLine(308, 200, 308, 208, WHITE);
  tft.drawLine(309, 209, 319, 219, WHITE);
  tft.drawLine(281, 219, 283, 219, WHITE);
  tft.drawLine(316, 219, 318, 219, WHITE);
  tft.drawRect(284, 219, 32, 21, WHITE);
  tft.drawRect(295, 225, 10, 15, WHITE);
}
void drawhomeiconred() { // draws a red home icon
  tft.drawLine(280, 219, 299, 200, RED);
  tft.drawLine(300, 200, 304, 204, RED);
  tft.drawLine(304, 203, 304, 200, RED);
  tft.drawLine(305, 200, 307, 200, RED);
  tft.drawLine(308, 200, 308, 208, RED);
  tft.drawLine(309, 209, 319, 219, RED);
  tft.drawLine(281, 219, 283, 219, RED);
  tft.drawLine(316, 219, 318, 219, RED);
  tft.drawRect(284, 219, 32, 21, RED);
  tft.drawRect(295, 225, 10, 15, RED);
}
void clearmessage() {
  tft.fillRect(12, 213, 226, 16, BLACK); // black out the inside of the message box
}
void drawbatt() {
  battv = getBandgap(); // read the voltage
  if (battv < battold) { // if the voltage goes down, erase the inside of the battery
    tft.fillRect(298, 2, 18, 6, BLACK);
  }
  battfill = map(battv, 300, 415, 2, 18); // map the battery voltage 300 is the low, 415 is the high
  battfill = constrain(battfill, 2, 18); // constrains batt display limits
  if (battfill > 7) { // if the battfill value is between 8 and 18, fill with green
    tft.fillRect(298, 2, battfill, 6, GREEN);
  }
  else { // if the battfill value is below 8, fill with red
    tft.fillRect(298, 2, battfill, 6, RED);
  }
  battold = battv; // this helps determine if redrawing the battfill area is necessary
}


// Only used for identifier is this case. if your not using comment out save some space....
// Copy string from flash to serial port
// Source string MUST be inside a PSTR() declaration!
void progmemPrint(const char *str) {
  char c;
  while(c = pgm_read_byte(str++)) Serial.print(c);
}

// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  progmemPrint(str);
  Serial.println();
}
 void setRotation(uint8_t rotation);
