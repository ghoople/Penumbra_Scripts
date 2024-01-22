// This code is for an arduino to read incoming data from the clear core.
// Uno uses pins RX: D0 and TX: D1  
// Connect the USB cable to the arduino to read data on the serial port. 

#include <Arduino.h>

int position;
int intensity;
char data;

void setup() {
  // Start serial communication on Serial with a baud rate of 9600 using the TTL pins
  Serial.begin(9600);

  // Initialize variables
  position = 0;
  intensity = 0;

  // Configure Pin 13 (on board LED) to be an output
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  delay(1000);
}

void loop() {
  // Check if there is data available to read on Serial1 (TTL pins)
  if (Serial.available() > 0) {
    // Read the incoming message until a newline character is received
    // String message = Serial.readStringUntil('\n'); // THIS COULD BE THE PROBLEM, IT MAY NOT BE OUTPUTTING \n TRY READING A SINGLE BYTE INSTEAD AS DONE IN OTHER CODE. 
    //Serial.println("Received Message");
    
    // Parse the message into two integers separated by a comma
    //sscanf(message.c_str(), "%d,%d", &position, &intensity);
    //Serial.print(position);
    //Serial.print(" and ");
    //Serial.println(intensity);

    data = Serial.read();

    Serial.println(data);
  }

  if(data==12){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }

    if(data==1000){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }

  delay(5000);

}
