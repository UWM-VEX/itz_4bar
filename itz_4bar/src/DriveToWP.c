/*
 * DriveToWP.c
 *
 *  Created on: Jul 27, 2015
 *      Author: Erik
 */

#include "main.h"

DriveToWPProperties * initDriveToWPProperties(Drive* drive, double magnitudeDB,
		double magnitudeBreakingDistance, long magnitudeRampUpTime, int magnitudeMaxSpeed,
		int magnitudeMinSpeed, int rotationDB, int rotationBreakingDistance, int rotationMaxSpeed,
		int rotationMinSpeed, double wheelDiameter, int gyroInverted, unsigned long holdTime)
{
	DriveToWPProperties * newDriveToWPProperties = malloc(sizeof(DriveToWPProperties));

	newDriveToWPProperties->drive = drive;
	newDriveToWPProperties->magnitudeDB = magnitudeDB;
	newDriveToWPProperties->magnitudeBreakingDistance = magnitudeBreakingDistance;
	newDriveToWPProperties->magnitudeRampUpTime = magnitudeRampUpTime;
	newDriveToWPProperties->magnitudeMaxSpeed = magnitudeMaxSpeed;
	newDriveToWPProperties->magnitudeMinSpeed = magnitudeMinSpeed;
	newDriveToWPProperties->rotationDB = rotationDB;
	newDriveToWPProperties->rotationBreakingDistance = rotationBreakingDistance;
	newDriveToWPProperties->rotationMaxSpeed = rotationMaxSpeed;
	newDriveToWPProperties->rotationMinSpeed = rotationMinSpeed;
	newDriveToWPProperties->wheelDiameter = wheelDiameter;
	newDriveToWPProperties->gyroInverted = gyroInverted;
	newDriveToWPProperties->holdTime = holdTime;

	return newDriveToWPProperties;
}

DriveToWP * initDriveToWP(DriveToWPProperties * properties, double distance, int rotation)
{
	DriveToWP * newDriveToWP = malloc(sizeof(DriveToWP));
	newDriveToWP->distance = distance;
	newDriveToWP->rotation = rotation;
	newDriveToWP->properties = properties;
	newDriveToWP->reachedDistance = 0;
	newDriveToWP->reachedRotation = 0;
	newDriveToWP->timeAchievedGoodLocation = 0;
	newDriveToWP->isFirstTime = 1;
	return newDriveToWP;
}

void driveToWP(DriveToWP * step)
{
	if(step->isFirstTime)
	{
		step->firstLeftEncoder = encoderGet(step->properties->drive->leftEncoder);
		step->firstRightEncoder = encoderGet(step->properties->drive->rightEncoder);
		step->firstGyro = gyroGet(step->properties->drive->gyro);
		step->isFirstTime = 0;
	}

	lcdPrint(uart1, 1, "L: %d", encoderGet(step->properties->drive->leftEncoder));
	lcdPrint(uart1, 2, "R: %d", encoderGet(step->properties->drive->rightEncoder));
	//lcdPrint(uart1, 1, "Gyro: %d", gyroGet(step->properties->drive.gyro));

	double averageMagEncoder = ((double)((encoderGet(step->properties->drive->leftEncoder)
			- step->firstLeftEncoder)
			+ (encoderGet(step->properties->drive->rightEncoder)
					- step->firstRightEncoder)) / 2);
	double distancePV = encoderToInches(averageMagEncoder,
			step->properties->wheelDiameter);
	int rotationPV = gyroGet(step->properties->drive->gyro) - step->firstGyro;

	//lcdPrint(uart1, 1, "Dist: %f", distancePV);

	if(step->properties->gyroInverted)
	{
		rotationPV *= -1;
	}

	double distanceError = step->distance - distancePV;
	int rotationError = step->rotation - rotationPV;

	//lcdPrint(uart1, 2, "Rot: %d", rotationError);

	int inDistanceDB = (absDouble(distanceError) < step->properties->magnitudeDB);

	/*if(absDouble(step->distance) < 0.5 && abs(step->rotation) == 0)
	{
		inDistanceDB = 1;
	}*/

	if(inDistanceDB)
	{
		step->reachedDistance = 1;
	}

	int inRotationDB = abs(rotationError) < step->properties->rotationDB;

	if(inRotationDB)
	{
		step->reachedRotation = 1;
	}

	int magnitude = 0;
	int rotation = 0;

	// If it has not reached both its distance and rotation targets
	if( ! (step->reachedDistance && step->reachedRotation))
	{
		//lcdSetText(uart1, 1, "Case 1");

		if(inDistanceDB)
		{
			//lcdSetText(uart1, 1, "Mag: DB");
			magnitude = 0;
		}
		else if(absDouble(distanceError) < step->properties->magnitudeBreakingDistance)
		{
			//lcdSetText(uart1, 1, "Mag: Break");
			// slow down
			// magnitude = (Vmax - Vmin)(SP - PV)/Breaking Distance + Vmin
			magnitude = (int) ((step->properties->magnitudeMaxSpeed -
					step->properties->magnitudeMinSpeed) * (distanceError)) /
							step->properties->magnitudeBreakingDistance +
							step->properties->magnitudeMinSpeed;
		}
		else if(autonomousInfo.elapsedTime < step->properties->magnitudeRampUpTime)
		{
			//lcdSetText(uart1, 1, "Mag: Accel");
			// speed up
			// magnitude = (Vmax - Vmin)*t/ramp up time + Vmin
			magnitude = (int) ((step->properties->magnitudeMaxSpeed -
					step->properties->magnitudeMinSpeed) * autonomousInfo.elapsedTime
							/ step->properties->magnitudeRampUpTime +
							step->properties->magnitudeMinSpeed);
		}
		else
		{
			//lcdSetText(uart1, 1, "Mag: Coast");
			// coast
			magnitude = step->properties->magnitudeMaxSpeed;
		}

		if(inRotationDB)
		{
			//lcdSetText(uart1, 2, "Rot: DB");
			// no rotation
			rotation = 0;
		}
		else if(abs(rotationError) < step->properties->rotationBreakingDistance)
		{
			//lcdSetText(uart1, 2, "Rot: Slow");
			// turn slowly
			rotation = step->properties->rotationMinSpeed;
		}
		else
		{
			//lcdSetText(uart1, 2, "Rot: Fast");
			// turn quickly
			rotation = step->properties->rotationMaxSpeed;
		}

		step->isFinished = 0;
	}
	else
	{
		//lcdSetText(uart1, 1, "Case 2");

		// If at some point we've reached a good distance and a good rotation
		int goodDistance = 0;

		// Check that we're at a good distance, if we're not, slowly move to a good distance
		if(absDouble(distanceError) < step->properties->magnitudeDB)
		{
			//lcdSetText(uart1, 1, "Mag: Good");
			goodDistance = 1;
		}
		else
		{
			//lcdSetText(uart1, 1, "Mag: Adj");
			magnitude = step->properties->magnitudeMinSpeed;
		}

		int goodRotation = 0;

		// Check that we're at a good rotation, if we're not, slowly move to a good rotation
		if(abs(rotationError) < step->properties->rotationDB)
		{
			//lcdSetText(uart1, 2, "Rot: Good");
			goodRotation = 1;
		}
		else
		{
			//lcdSetText(uart1, 2, "Rot: Adj");
			rotation = step->properties->rotationMinSpeed;
		}

		if(goodDistance && goodRotation)
		{
			if(step->timeAchievedGoodLocation == 0)
			{
				step->timeAchievedGoodLocation = millis();
			}

			if(millis() - step->timeAchievedGoodLocation > step->properties->holdTime)
			{
				step->isFinished = 1;
			}
		}
		else
		{
			step->timeAchievedGoodLocation = 0;
		}
	}

	if(magnitude != 0)
	{
		magnitude = limit(magnitude, step->properties->magnitudeMaxSpeed,
				step->properties->magnitudeMinSpeed);
	}

	if(rotation != 0)
	{
		rotation = limit(rotation, step->properties->rotationMaxSpeed,
				step->properties->rotationMinSpeed);
	}

	if(distanceError < 0)
	{
		magnitude *= -1;
	}

	if(rotationError < 0)
	{
		rotation *= -1;
	}

	//lcdPrint(uart1, 1, "%f", directionPV);
	//lcdPrint(uart1, 2, "%f", directionError);

	arcadeDrive(step->properties->drive, magnitude, rotation);
}
