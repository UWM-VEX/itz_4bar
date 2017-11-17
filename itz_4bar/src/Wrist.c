#include "main.h"

Wrist* initWristIO(int frontLimitSwitch, int rearLimitSwitch)
{
  Wrist* newWrist = (Wrist*) malloc(sizeof(Wrist));

  pinMode(frontLimitSwitch, INPUT);
  pinMode(rearLimitSwitch, INPUT);

  newWrist->frontLimitSwitch = frontLimitSwitch;
  newWrist->rearLimitSwitch = rearLimitSwitch;

  return newWrist;
}

void initWrist(Wrist* wrist, PantherMotor leftMotor, PantherMotor rightMotor, Encoder encoder)
{
  wrist->leftMotor = leftMotor;
  wrist->rightMotor = rightMotor;
  wrist->encoder = encoder;
}

void wristAtSpeed(Wrist* wrist, int speed, bool safetyOverride)
{
  if( ! safetyOverride)
  {
    if(digitalRead(wrist->frontLimitSwitch) == HIGH) // Front limit switch triggered
    {
      speed = limit(speed, 0, -127);
    }
    else if(digitalRead(wrist->rearLimitSwitch) == HIGH) // Rear limit switch triggered
    {
      speed = limit(speed, 127, 0);
    }
  }

  setPantherMotor(wrist->leftMotor, speed);
  setPantherMotor(wrist->rightMotor, speed);
}
