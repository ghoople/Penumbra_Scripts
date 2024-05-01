#include <Arduino.h>
#include <DmxSimple.h>

int channels = 4;
int brightness;

void setup() {
// Set DMX module to master mode
pinMode(2,OUTPUT);
digitalWrite(2,HIGH);
Serial.begin(9600);

// Set the pin the module will receive commands on
DmxSimple.usePin(4);

// Set the maximum number of channels I have on my system. 
DmxSimple.maxChannel(channels);
Serial.println("Arudino Ready");
}

void loop(){
  
  for (brightness = 0; brightness <= 255; brightness++) {
    DmxSimple.write(1, brightness);
    DmxSimple.write(3, brightness);
    delay(100);
    Serial.println(brightness);
    }
}