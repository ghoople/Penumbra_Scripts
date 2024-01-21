#include <Arduino.h>
#include <DmxSimple.h>

int channels = 4;
int brightness;

void setup() {
// Set DMX module to master mode
pinMode(2,OUTPUT);
digitalWrite(2,HIGH);

// Set the pin the module will receive commands on
DmxSimple.usePin(4);

// Set the maximum number of channels I have on my system. 
DmxSimple.maxChannel(channels);
Serial.println("Arudino Ready");
}

void loop(){
  /* Simple loop to ramp up brightness */
  for (brightness = 0; brightness <= 255; brightness++) {
    
    /* Update DMX channel 1 to new brightness */
    DmxSimple.write(4, brightness);
    
    /* Small delay to slow down the ramping */
    delay(10);
    Serial.println(brightness);
  }
}