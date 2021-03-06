// Notes: Changed LED outpin to pin 11 so I can use PWM 
const int TxPin = 7; 
const int ledPin = 10; 
const int lineDelay = 500; // Time between first line sent to LCD and second line.
const int clearDelay = 2000; // time after a message is written, before we clear it const 
int nextMesDelay = 3000; // Time after we clear a message before we flash the backlight

const int LED2 = A5; // TESTING LED TO REPRESENT BACKLIGHT

int ledState = LOW; // ledState used to set the LED 
long previousMillis = 0;  // will store last time LED was updated 
long interval = 1000;

/* VARIABLES FOR LED FADE */
int LEDFadeValue = 0; 
unsigned long fadeTime = 0; 
int x = 0; 
int y = 0;

/** VARIABLES FOR Backlight Flash **/ 
unsigned long bFTime = 0; int bF = 1; int flashCounter = 0; int flashCount = 11; // Number of times the LCD backligt flashes between messages. 
int messageVar = 0;

/** VARIABLES for LCD Message Write **/ 
int DisplayCount = 1;

// THIS IS WHERE YOU ADD ADDITIONAL MESSAGES. THEY TAKE THE FORMAT OF: {"LCD LINE 1 TEXT", "LCD LINE 2 TEXT"}
char* displayText[][2]={ 
  {"Hi, my name is ", " NOAH-BOT "}, 
  {" Trick or ", " Treat? "}, 
  {" ERROR...", " ERROR!!"}, 
  {" NEED CANDY INPUT..."," "}, 
  {" HAPPY ", " HALLOWEEN! "}, 
  {" Test Message 1p1", " Test Message 1p2"}, 
  {" Test Message 2p1", " Test Message 2p2"} 
}; 

unsigned long LCDTime = 0; int NumMessages = 0;

#include <SoftwareSerial.h>

SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

void setup() {
    
    pinMode(TxPin, OUTPUT); digitalWrite(TxPin, HIGH); pinMode(ledPin, OUTPUT); 
    //pinMode(LED2, OUTPUT)
    
    mySerial.begin(9600); 
    delay(100); 
    mySerial.write(12); // Clear
    mySerial.write(17); // Turn backlight on 
    mySerial.write(22); // Turn backlight on 
    delay(5); // Required delay
    
    mySerial.print(" NOAH-BOT "); // First line // 
    delay(3000); // Wait 3 seconds
    mySerial.write(13); // Form feed 
    mySerial.print(" Version 2.0 "); // Second line // 
    delay(3000); 
    mySerial.write(12); // Clear
    fadeTime = millis(); 
    flashCounter = 0; 
    DisplayCount = 1; 
    NumMessages = sizeof(displayText)/sizeof(displayText[0]); 

}

void loop() {

  ledFade(); if(flashCounter == flashCount) LCDMessage(); // If we've finished Flashing, send the next message to the LCD backlightFlash(); 

}

void LCDMessage() { 

	unsigned long currenTime = millis();

	switch (DisplayCount) { 

		case 1: mySerial.write(17); messageVar = random(0, NumMessages); // Select a random string from the array
		mySerial.print(displayText[messageVar][0]); // Print First line to LCD 
		LCDTime = millis(); 
		DisplayCount = 2; 
		break; 

		case 2: if(currenTime > (LCDTime + lineDelay)) {  // Delay for lineDelay sec before displaying next line, not sure why.  
			mySerial.write(13); // Carriage Return 
			mySerial.print(displayText[messageVar][1]); 
			LCDTime = millis(); 
			DisplayCount = 3; 
		}
		break; 

		case 3: if(currenTime > (LCDTime + clearDelay)) { 
			mySerial.write(12); 
			mySerial.write(18); 
			DisplayCount = 4; 
			LCDTime = millis(); 
		} 

		case 4: if(currenTime > (LCDTime + nextMesDelay)) { 
			flashCounter = 0; 
			flashCount = random(3,20); // Set the number of Backlight flashes to a random number of max value 20 
			DisplayCount = 1; 
		} 
		break; 
		default: DisplayCount = 1; 
		break; 
	}
}

/* Function for flasing the LCD backlight on and off */ \
void backlightFlash() { 
	unsigned long currenTime = millis();

	if(flashCounter<flashCount) { 
		switch (bF) { 
			case 1: mySerial.write("17");
			digitalWrite(LED2, HIGH); // Used for Testing 
			bF = 2; 
			bFTime = millis(); 
			break; 

			case 2: if(currenTime > (bFTime + 100)) bF=3; 
			break; 

			case 3: mySerial.write("18"); //
			digitalWrite(LED2, LOW); // Used for Testing 
			bF = 4; 
			bFTime = millis(); 

			case 4: if(currenTime > (bFTime + 100)) { 
				bF=1; flashCounter++; 
			} 
			break; 
			default: bF = 1; 
			break; 
		} 
	}

}

/** Function for Fading the LED on and off without delaying the code **/ 
void ledFade() {

    unsigned long currenTime = millis();

    if(currenTime > (fadeTime + 5)) {
        analogWrite(ledPin, x); 
        if(y) {x++;} if(!y) {
            x--;
        } 
        if(x>250) {
            y=0;
        } 
        if(x<10) {
            y=1;
        } 
        fadeTime = millis(); 
    } 
}
