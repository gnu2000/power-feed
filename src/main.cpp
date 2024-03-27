// Simple power feed controller, based on code by user groundFungus in this post: https://forum.arduino.cc/t/accelstepper-speed-control/899272

#include <Arduino.h>
#include <AccelStepper.h>

int Stepper1Pulse = 9;  // **** for CNC shield
int Stepper1Direction = 8;  // **** for CNC shield
const byte enablePin = 10;   // **** for CNC shield
int speedpot = A0;

// Define some pins for user controls
int onoffPin = 3;
int rapidPin = 4;
int directionPin = 2;

// Motor stuff
int Motor1speed = 0;
int speedmin = 0;
int speedmax = 1000;
AccelStepper step1(1, Stepper1Pulse, Stepper1Direction);

// put function declarations here:
void updateSwitches();

// Setup
void setup()
{
   step1.setMaxSpeed (speedmax);
   step1.setSpeed(0);
   step1.setAcceleration(500);
   pinMode(enablePin, OUTPUT);  // **** for CNC shield
   digitalWrite(enablePin, LOW);   // **** for CNC shield
   digitalWrite(Stepper1Direction, HIGH); // CW
   
   // Setup pins for controls
   pinMode( rapidPin, INPUT_PULLUP );
   pinMode( onoffPin, INPUT_PULLUP );
   pinMode( directionPin, INPUT_PULLUP );

   // Start serial
   Serial.begin(115200); // ************** faster baud rate
   //Serial.println("Running: StepperDriverTest");

}

void loop()
{
   static unsigned long timer = 0;
   unsigned long interval = 100;
   if (millis() - timer >= interval)
   {
      timer = millis();

      Motor1speed = map((analogRead(speedpot)), 1023, 0, speedmin, -speedmax); // for some reason my pot is reversed, also set speedmax to negative to get correct direction on the switch

      updateSwitches();

      step1.setSpeed(Motor1speed);
      //Serial.println(Motor1speed);
   }
      step1.runSpeed();
}

void updateSwitches() // Function to update the states of the controls
{
    // Read the switch states
    int onoff = digitalRead(onoffPin);
    int rapid = digitalRead(rapidPin);
    int direction = digitalRead(directionPin);
  
    // Rapid logic
    if (rapid == LOW) {
        Motor1speed = -speedmax;
    }

    // On/Off logic
    if (onoff == LOW) {
        Motor1speed = speedmin;
        digitalWrite(enablePin, HIGH);
    } else {
        digitalWrite(enablePin, LOW);
    }
    
    // Direction logic
    if (direction == LOW) {
        Motor1speed = Motor1speed * -1;
    }
}
