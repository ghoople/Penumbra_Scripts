// This code is used to test if the wheel properly is controlling the motor. 
#include <Arduino.h>
#include <ClearCore.h>

// Specifies which motor to move.
// Options are: ConnectorM0, ConnectorM1, ConnectorM2, or ConnectorM3.
#define motor ConnectorM0

// Select the baud rate to match the target serial device
#define baudRate 9600

// Variable for encoder counting
unsigned long startTime;
unsigned long motorStartTime;
unsigned long currentTime;
unsigned long motorTime;
float averageVelocity;

float totalVelocity = 0.0;
int readingsCount = 0;

// Variables to store encoder state
int32_t Wheel_position = 0;
int32_t last_Wheel_position = 0;
int32_t Wheel_velocity = 0;
int32_t indexPosition = 0;
int32_t lastIndexPosition = 0;
bool quadratureError = false;

// Set to true if index detection should occur on the falling edge, rather than the rising edge.
bool indexInverted = false; 
// Set to true if the sense of encoder direction should be inverted.
bool swapDirection = false; 

// Define the physical relationship between steps and light position
// See "Penumbra Motor Calculations" google sheet for value calculator
int Pos_Bottom = 0; 
int Pos_Top = 7858;

// Create Variables for the state of the inputs
int User_Speed = 0; 

// Define the initial velocity and acceleration limits to be used for default moves
int velocityLimit = 2300; // pulses per sec
int accelerationLimit = 4600; // pulses per sec^2

void setup() {
    // This setup function only runs once

    // Sets the input clocking rate.
    MotorMgr.MotorInputClocking(MotorManager::CLOCK_RATE_LOW);

    // Sets all motor connectors into step and direction mode.
    MotorMgr.MotorModeSet(MotorManager::MOTOR_ALL,
                          Connector::CPM_MODE_STEP_AND_DIR);

    // Sets the maximum velocity for each move
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

    // Enables output to the motor (must have, even if not using the enable pin on the motor)
    motor.EnableRequest(true);

    // Enable the encoder input feature
    EncoderIn.Enable(true);
    // Set the encoder direction
    EncoderIn.SwapDirection(swapDirection);
    // Set the sense of index detection (true = rising edge, false = falling edge)
    EncoderIn.IndexInverted(indexInverted);

    // Zero the encoder position to match the zero of the motor. 
    EncoderIn.Position(0);
    motor.PositionRefSet(0);   

    Serial.println("Setup Complete");
}

void loop() {

  Wheel_position = EncoderIn.Position(); // Read the encoder position
  Wheel_velocity = EncoderIn.Velocity(); // Read the encoder velocity
  //Serial.print("Wheel Position: ");
  //Serial.println(Wheel_position);
  //Serial.print("Wheel Velocity: ");


  totalVelocity += Wheel_velocity;
  readingsCount++;

  currentTime = millis();
  motorTime = currentTime;
  
  // Check if 0.1 seconds have passed
  if (currentTime - startTime >= 100) {
    // Calculate average velocity over 0.1 seconds
    averageVelocity = totalVelocity / readingsCount;

    // Print the average velocity
    Serial.print("Average Wheel Velocity: ");
    Serial.println(averageVelocity);

    // Reset variables for the next interval
    totalVelocity = 0.0;
    readingsCount = 0;
    startTime = currentTime;
  }

  // Map the wheel velocity (counts/second) to an appropriate motor speed (Pulses/sec).
  // Encoder: 
    // 1 revolution in 5 seconds on the wheel = 200 counts/second
    // 250 RPM = 4.2 rev/s (Seems hard to imagine going that fast) = 42,000 counts/second  
  // Motor: (Pulse output)
    // 0 - 3000 seems reasonable to start, may want to adjust
  User_Speed = map(abs(averageVelocity),0,42000,0,3000);


  // This Loop Occurs Every 200 ms. Updating faster seems to cause the motor to freak out. 
  if (motorTime - motorStartTime >= 200) {
    if(Wheel_position > last_Wheel_position){ // Go UP
      motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
      motor.Move(Pos_Top, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
      //Serial.print("Moving up at user speed: ");
      //Serial.println(User_Speed);
    }
    else{ // Go Down
      motor.VelMax(User_Speed); // Sets the maximum velocity for this move based on the potentiometer
      motor.Move(Pos_Bottom, MotorDriver::MOVE_TARGET_ABSOLUTE); // Commands the motor move (with no waiting step before commanding additional moves!)
    }
    last_Wheel_position = Wheel_position;
    motorStartTime = motorTime; 
  }

}











