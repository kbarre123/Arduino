int led_1 = 2;
int led_2 = 3;
int led_3 = 4;
int led_4 = 5;
int led_5 = 6;
int led_6 = 7;
int led_7 = 8;
int led_8 = 9;
int led_9 = 10;
int led_10 = 11;
int led_11 = 12;
int led_12 = 13;

int potPin = A0;
int potLevel = 0;

// the setup routine runs once when you press reset:
void setup() { 

    Serial.begin(9600);
    
    // initialize the digital pin as an output.
    for (int thisPin = 2; thisPin < 14; thisPin++)
    {
        pinMode(thisPin, OUTPUT);
    }
    
    pinMode(potPin, INPUT);
}

void loop() 
{
    potLevel = analogRead(potPin);
    potLevel = map(potLevel, 0, 1023, 0, 255);
    Serial.println(potLevel);
    
    for (int thisPin = 2; thisPin < 14; thisPin++)
    {
        digitalWrite(thisPin, HIGH);
        delay(potLevel);
        digitalWrite(thisPin, LOW);
    }    
    
    /*for (int thisPin = 2; thisPin < 14; thisPin++)
    {
        digitalWrite(thisPin, LOW);
        delay(timer);
    }*/   
}

