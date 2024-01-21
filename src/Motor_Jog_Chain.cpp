/*
 * Title: Motor Jog Control (No Home)
 * This is for manual movements only.
 */

#include <Arduino.h>
#include "ClearCore.h"

// Specifies which motor to move.
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0

// Select the baud rate to match the target serial device
#define baudRate 9600

//Configure Inputs
#define UpButton IO0
#define DownButton IO1
#define SpeedPot A12 //For analog reads must use A-9, A-10, A-11, or A12 

// Create Variables for the state of the inputs
int State_UpButton = 0; 
int State_DownButton = 0; 
int Value_SpeedPot = 0; 
int User_Speed = 0;

// Define the initial velocity and acceleration limits to be used for default moves
int velocityLimit = 5000; // pulses per sec
int accelerationLimit = 9000; // pulses per sec^2 . May want to play with this some. 

void setup() {
    // This setup function only runs once

    // Sets the input clocking rate.
    MotorMgr.MotorInputClocking(MotorManager::CLOCK_RATE_LOW);

    // Sets all motor connectors into step and direction mode.
    MotorMgr.MotorModeSet(MotorManager::MOTOR_ALL,
                          Connector::CPM_MODE_STEP_AND_DIR);

    // Sets the initial velocity for each move
    motor.VelMax(velocityLimit);

    // Set the maximum acceleration for each move
    motor.AccelMax(accelerationLimit);

    // Sets up serial communication and waits up to 5 seconds for a port to open.
    // Serial communication is not required for this example to run.
    Serial.begin(baudRate);
    uint32_t timeout = 5000;
    uint32_t startTime = millis();
    while (!Serial && millis() - startTime < timeout) {
        continue;
    }

    // Configure the input pins
    pinMode(UpButton, INPUT);
    pinMode(DownButton, INPUT);
    pinMode(SpeedPot, INPUT);

    // Enables output to the motor (must have, even if not using the enable pin on the motor)
    motor.EnableRequest(true);

    Serial.println("Motor Ready");
}

void loop() {

  //Read the sensors
  State_UpButton = digitalRead(UpButton);
  State_DownButton = digitalRead(DownButton);
  Value_SpeedPot = analogRead(SpeedPot); 

  // Map the SpeedPot to an appropriate acceleration
  // Analog read will return a value from 0 to 4095. My current pot & resistors goes to 3371. Need to test. 
  // Map onto my desired speed in Pulses/Sec (Range of 500 - 3000 seems reasonable to start)
  User_Speed = map(Value_SpeedPot,0,3380,500,velocityLimit);

  if(State_UpButton == HIGH){
    motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
    motor.MoveVelocity(User_Speed);
  }
  else if(State_DownButton == HIGH){
    motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
    motor.MoveVelocity(-1*User_Speed);
    //Serial.print("Moving down at user speed: ");
    //Serial.println(User_Speed);
  }
  else{ // Turn off the motor if no buttons currently depressed
    motor.MoveStopDecel(4000); // Stops the motor using the higher of the current acceleration value or 4000 pulses/s^2
  }
    
}











