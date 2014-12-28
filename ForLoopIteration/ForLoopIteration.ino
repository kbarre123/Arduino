int ledPin_01 = A4;
int ledPin_02 = A5;
int ledPin_03 = 2;
int ledPin_04 = 3;
int ledPin_05 = 4;
int ledPin_06 = 5;
int ledPin_07 = 6;
int ledPin_08 = 7;

int channels[] = 
{
  ledPin_01, ledPin_02, ledPin_03, ledPin_04, ledPin_05, ledPin_06, ledPin_07, ledPin_08
};

int channel_count = 8;

int timer = 100;           // The higher the number, the slower the timing.

void setup() 
{
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < channel_count; thisPin++)
  {
    pinMode(channels[thisPin], OUTPUT);      
  }
}

void loop() 
{
  // loop from the lowest pin to the highest:
  for (int thisPin = 0; thisPin < channel_count; thisPin++) 
  { 
    // turn the pin on:
    digitalWrite(channels[thisPin], HIGH);   
    delay(timer);                  
    // turn the pin off:
    digitalWrite(channels[thisPin], LOW);
    delay(timer);    
  }

  // loop from the highest pin to the lowest:
  for (int thisPin = (channel_count - 1); thisPin >= 0; thisPin--) 
  { 
    // turn the pin on:
    digitalWrite(channels[thisPin], HIGH);
    delay(timer);
    // turn the pin off:
    digitalWrite(channels[thisPin], LOW);
    delay(timer);
  }
}
