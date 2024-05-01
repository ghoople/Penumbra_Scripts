/*
 * Title: Penumbra Manual Control 
 * This is for manual movements only.
 */

#include <Arduino.h>
#include "ClearCore.h"

// Specifies which motor to move.
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0

// Select the baud rate to match the target serial device
#define baudRate 9600

// Configure Interrupts
// Pins that support digital interrupts are:
// DI-6, DI-7, DI-8, A-9, A-10, A-11, A-12
#define BotInterruptPin A11

//Configure Inputs
#define UpButton IO0
#define DownButton IO1
#define SpeedPot A10 //For analog reads must use A-9, A-10, A-11, or A12 

// Create Variables for the state of the inputs
int State_UpButton = 0; 
int State_DownButton = 0; 
int Value_SpeedPot = 0; 
int User_Speed = 0; 

// Define the initial velocity and acceleration limits to be used for default moves
int velocityLimit = 2000; // pulses per sec
int accelerationLimit = 4000; // pulses per sec^2

// Define the physical relationship between steps and light position
// See "Penumbra Motor Calculations" google sheet for value calculator
int Pos_Bottom = 0; 
int Pos_Top = 12036; // I think this should be close to 12740, will move manually to find it. 
int Pos_Middle = Pos_Top/2;
int Home_Offset = 95; // This is about 1 inch. Need to test it. 

// Define this at the top of your file or function
unsigned long lastCheckTime = 0;

// Declare functions included in this file
void BottomHardStop(); // Interrupt for when the bottom hard stop is triggered

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
    pinMode(BotInterruptPin, INPUT);
    pinMode(UpButton, INPUT);
    pinMode(DownButton, INPUT);
    pinMode(SpeedPot, INPUT);

    // Setup Interrupt for Hard Stop at bottom
    // Attaches to Digital Input 6 
    attachInterrupt(digitalPinToInterrupt(BotInterruptPin), BottomHardStop, RISING);

    // Enables output to the motor (must have, even if not using the enable pin on the motor)
    motor.EnableRequest(true);

    motor.MoveVelocity(-500); // Move down slowly, should  stop when the hard stop is tripped.  
    while (!motor.StepsComplete()) {
        continue;
    }

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
  User_Speed = map(Value_SpeedPot,0,3380,500,3000);

  if(State_UpButton == HIGH){
    motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
    motor.Move(Pos_Top, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
    //Serial.print("Moving up at user speed: ");
    //Serial.println(User_Speed);

  }
  else if(State_DownButton == HIGH){
    motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
    motor.Move(Pos_Bottom, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
    //Serial.print("Moving down at user speed: ");
    //Serial.println(User_Speed);
  }
  else{ // Turn off the motor if no buttons currently depressed
    motor.MoveStopDecel(4000); // Stops the motor using the higher of the current acceleration value or 4000 pulses/s^2
  }

  // Output my current position every 2 seconds
  unsigned long currentTime = millis();
  if(currentTime - lastCheckTime >= 2000) {
      lastCheckTime = currentTime;
      long motorPosition = motor.PositionRefCommanded();
      Serial.print("Current motor position: ");
      Serial.println(motorPosition);
  }
    
}

/*------------------------------------------------------------------------------
 * HardStop
 *
 *    Called by the interrupt when the hardstop switch is triggered. Resets the motor home to just above the hardstop
 *
 * Parameters:
 *    None
 *
 * Returns: None
 */
void BottomHardStop() {
    // Called via interrupt when the hard stop pin is tripped. 

    // Deattach the hard stop pin so it doesn't trip twice (had this problem, known as "bouncing")
    detachInterrupt(digitalPinToInterrupt(BotInterruptPin)); // detach the interrupt

    motor.MoveStopAbrupt();

    Serial.println("Bottom Hard Stop Triggered");
    
    // Move away from the hard stop
    motor.VelMax(2000); // Move at 2000 pulse/sec away from hard stop

    motor.Move(Home_Offset);
        Serial.println("Moving away from the hard stop... Waiting for the step output to finish...");
    while (!motor.StepsComplete()) {
        continue;
    }

    // Zero the motor's reference position after homing to allow for accurate absolute position moves

    motor.PositionRefSet(0);     

    // Reattach the interrupt

    attachInterrupt(digitalPinToInterrupt(BotInterruptPin), BottomHardStop, RISING);

}












