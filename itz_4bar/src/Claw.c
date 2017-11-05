#include "main.h"

Claw* initClaw(PantherMotor motor)
{
  Claw* newClaw = (Claw*) malloc(sizeof(Claw));

  return newClaw;
}

void clawAtSpeed(Claw* claw, int speed)
{
  setPantherMotor(claw->motor, speed);
}

void openClaw(Claw* claw)
{
  clawAtSpeed(claw, 127);
}

void closeClaw(Claw* claw)
{
  clawAtSpeed(claw, -127);
}

void stopClaw(Claw* claw)
{
  clawAtSpeed(claw, 0);
}
