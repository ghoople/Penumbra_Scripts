// Example 3 - Receive with start- and end-markers
// FML Serial works this way! The clear core can't drive the redboard pin 1 to 5V for some reason. Only drives it to 2.5 v

#include <Arduino.h>
#include <SoftwareSerial.h>

#define rxPin 3
#define txPin 4
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

const byte numChars = 32;
char receivedChars[numChars];

void recvWithStartEndMarkers();
void showNewData();

boolean newData = false;

void setup() {
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
    
    mySerial.begin(9600);
}

void loop() {
    recvWithStartEndMarkers();
    showNewData();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (mySerial.available() > 0 && newData == false) {
        rc = mySerial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
    }
}