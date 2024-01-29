#include <Arduino.h>
#include <DmxSimple.h>

int channels = 4;
int brightness;

void setup() {
// Set DMX module to master mode
pinMode(2,OUTPUT);
digitalWrite(2,HIGH);
Serial.begin(9600);

// Set the pin the module will send commands on
DmxSimple.usePin(4);

// Set the maximum number of channels I have on my system. 
DmxSimple.maxChannel(channels);
Serial.println("Arudino Ready");
DmxSimple.write(1, 0);
DmxSimple.write(2, 0);
}

void loop(){
  // Set the brightness of the Bulbs
DmxSimple.write(1, 30);
DmxSimple.write(3, 0);
delay(2000);
  // Set the brightness of the Bulbs
DmxSimple.write(1, 0);
DmxSimple.write(3, 30);
delay(2000);


}