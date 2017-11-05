/*
 * OI.c
 *
 *  Created on: Oct 29, 2015
 *      Author: Erik
 */

#include "main.h"

int OIGetDriveLeft()
{
	return enforceDeadband(joystickGetAnalog(1, 3), 0, 20);
}

int OIGetDriveRight()
{
	return enforceDeadband(joystickGetAnalog(1, 2), 0, 20);
}

int OIGetDriveY()
{
	return joystickGetAnalog(1, 3);
}

int OIGetDriveX()
{
	return joystickGetAnalog(1, 4);
}

int OIGetLift()
{
	return joystickGetAnalog(2,3);
}

int OIGetWrist()
{
	return joystickGetAnalog(2,4);
}

bool OIOpenClaw()
{
	return joystickGetDigital(2, 5, JOY_UP);
}

bool OICloseClaw()
{
	return joystickGetDigital(2, 6, JOY_UP);
}
