/*
 * Switch test program
 * Using a 3-prong, 2-state switch:
 *         _
 *      __| |__
 *     |       |
 *   __|_______|__
 *      |  |  |
 *      |  |  |
 *      |  |  |
 *      G  5V
 *      |  |___________________5V  
 *      |
 *      |______________________pin
 *      |
 *      |_____/\/\/\___________Ground
 *             10K 
 *
 * Only two wires are used. The center pin goes directly to 5V.
 * One ground goes directly to digital pin. From same row (but not obstructing the wire to the pin),
 * place a 10K resister with the other end going to ground.
 * See diagram here: http://www.arduino.cc/en/Tutorial/button
 */

#define SWITCH_PIN 13             // Switch connected to digital pin 13

void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pinMode(SWITCH_PIN, INPUT);    // sets the digital pin as input to read switch
}


void loop()                     // run over and over again
{
  Serial.print("Read switch input: ");
  Serial.println(digitalRead(SWITCH_PIN));    // Read the pin and display the value
  delay(100);
}
