
#include <FastLED.h>
#include <Arduino.h>

#define NUM_LEDS 144 // Three strips of 144 LEDs in series. 
// Must keep this below absolute limit of 1365 for hardware limit on Teensy. 
// With an Arduino Uno must be less than 500 for memory reasons, otherwise will need to go to Teensy.  
// Paul recommended limits are 1100 or 550 per pin, since the speed of communication limits the refresh rate to 30 or 60 Hz with those lengths.

#define LED_GRP1 6  // Data pin on the arduino
//#define LED_GRP2 10  // Data pin on the arduino
//#define LED_GRP3 11  // Data pin on the arduino

//Add a comment. 

CRGB leds[NUM_LEDS]; 

void setup() {
  FastLED.addLeds<WS2812B, LED_GRP1, GRB>(leds, NUM_LEDS);  // Adjust color order if needed, website says Ws2815 are GRB and run same code as WS2812B
//  FastLED.addLeds<WS2812B, LED_GRP2, GRB>(leds, NUM_LEDS);  
//  FastLED.addLeds<WS2812B, LED_GRP3, GRB>(leds, NUM_LEDS);  
  FastLED.setBrightness(20);  // Set maximum brightness
}

void loop() {

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;  // Set all LEDs to white
  }
  FastLED.show();  // Send the data to the LED strip
  delay(1000);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;  // Set all LEDs to red
  }
  FastLED.show();  // Send the data to the LED strip
  delay(1000);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;  // Set all LEDs to green
  }
  FastLED.show();  // Send the data to the LED strip
  delay(1000);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;  // Set all LEDs to blue
  }
  FastLED.show();  // Send the data to the LED strip
  delay(1000);

}
