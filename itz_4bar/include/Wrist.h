#ifndef INCLUDE_WRIST_H_
#define INCLUDE_WRIST_H_

#include "main.h"

struct Wrist{
  PantherMotor leftMotor;
  PantherMotor rightMotor;
} typedef Wrist;

Wrist* initWrist(PantherMotor leftMotor, PantherMotor rightMotor);
void wristAtSpeed(Wrist* wrist, int speed);

#endif
