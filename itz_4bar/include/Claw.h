#ifndef INCLUDE_CLAW_H_
#define INCLUDE_CLAW_H_

#include "main.h"

struct Claw{
  PantherMotor motor;
} typedef Claw;

Claw* initClaw(PantherMotor motor);
void openClaw(Claw* claw);
void closeClaw(Claw* claw);
void stopClaw(Claw* claw);
void clawAtSpeed(Claw* claw, int speed);

#endif
