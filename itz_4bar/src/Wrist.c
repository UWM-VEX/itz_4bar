#include "main.h"

Wrist* initWrist(PantherMotor leftMotor, PantherMotor rightMotor)
{
  Wrist* newWrist = (Wrist*) malloc(sizeof(Wrist));

  newWrist->leftMotor = leftMotor;
  newWrist->rightMotor = rightMotor;

  return newWrist;
}

void wristAtSpeed(Wrist* wrist, int speed)
{
  setPantherMotor(wrist->leftMotor, speed);
  setPantherMotor(wrist->rightMotor, speed);
}
