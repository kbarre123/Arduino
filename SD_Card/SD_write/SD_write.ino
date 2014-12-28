/* 
  SD card read/write
*/

// Include SD library
#include <SD.h>

File myFile;

void setup()
{
  Serial.begin(9600);
  
  Serial.print("Initializing card...");
  
  // Card-select pin is 10 on my SD shield
  pinMode(10, OUTPUT);
  
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return; // Stop program
  }
  Serial.println("initialization complete.");
  
  // Open the file.
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // If file opens, write to it.
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // Close the file
    myFile.close;
    Serial.println("Done.");
  }
  else {
    // If file didn't open, throw error
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // Nothing happens in the loop
}
