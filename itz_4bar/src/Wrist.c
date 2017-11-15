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

void wristAtSpeed(Wrist* wrist, int speed)
{
  setPantherMotor(wrist->leftMotor, speed);
  setPantherMotor(wrist->rightMotor, speed);
}
