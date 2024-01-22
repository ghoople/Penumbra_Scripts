// This code runs on the clear core and is used to test serial communications with an Arduino

#include <Arduino.h>
#include <ClearCore.h>

int position;
int brightness;

void setup() {
  // Start serial communication on Serial1 with a baud rate of 28800
  Serial1.begin(28800);
  Serial1.ttl(true); 
  while (!Serial1) {
      continue;
  }
  //Sets up USB serial communication and waits up to 5 seconds for a port to open.
  Serial.begin(9600);
  uint32_t timeout = 5000;
  uint32_t startTime = millis();
  while (!Serial && millis() - startTime < timeout) {
      continue;
  }

  // Initialize variables
  position = 0;
  brightness = 0;
  delay(5000);
}

void loop() {
  // Send an update to the usb serial port
  Serial.println("Sending Command");
  Serial.println(position);
  
  // Send the values via Serial1
  Serial1.print(position);
  Serial1.print(",");
  Serial1.println(brightness);

  delay(2000); // Wait two seconds so we don't send too much data to the Agent. 

  //Incrememnt the values
  position++;
  brightness++;
}
