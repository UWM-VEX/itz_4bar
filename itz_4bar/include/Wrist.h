#ifndef INCLUDE_WRIST_H_
#define INCLUDE_WRIST_H_

#include "main.h"

struct Wrist{
  PantherMotor leftMotor;
  PantherMotor rightMotor;
  int frontLimitSwitch;
  int rearLimitSwitch;
  Encoder encoder;
} typedef Wrist;

Wrist* initWristIO(int frontLimitSwitch, int rearLimitSwitch);
void initWrist(Wrist* wrist, PantherMotor leftMotor, PantherMotor rightMotor, Encoder encoder);
void wristAtSpeed(Wrist* wrist, int speed, bool safetyOverride);

#endif
