/*
 * OI.h
 *
 *  Created on: Oct 29, 2015
 *      Author: Erik
 */

#ifndef INCLUDE_OI_H_
#define INCLUDE_OI_H_

int OIGetDriveLeft();
int OIGetDriveRight();
int OIGetDriveY();
int OIGetDriveX();
int OIGetLift();
int OIGetWrist();
bool OIInRoller();
bool OIOutRoller();
bool OIElevatorUp();
bool OIElevatorDown();
bool OIPuncherOut();
bool OIPuncherIn();

#endif /* INCLUDE_OI_H_ */
