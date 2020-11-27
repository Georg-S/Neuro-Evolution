// Fill out your copyright notice in the Description page of Project Settings.

#include "SideAndGroundSensoric.h"

SideAndGroundSensoric::SideAndGroundSensoric(FCollisionQueryParams collisionParams)
{
	this->collisionParams = collisionParams;
}

std::vector<double> SideAndGroundSensoric::getDataFromSensors(const FVector& forwardVector, const FVector& actorLocation, UWorld* world)
{
	const FVector zVector = FVector(0.f, 0.f, 1.f);

	//Ground-Sensoric
	FVector frontDiagonalDown = forwardVector - zVector;
	frontDiagonalDown.Normalize();
	frontDiagonalDown *= this->sensorLength;

	FVector backDiagonalDown = -frontDiagonalDown;
	backDiagonalDown.Z = frontDiagonalDown.Z;

	FVector rightDiagonalDown = FVector::CrossProduct(zVector, forwardVector) - zVector;
	rightDiagonalDown.Normalize();
	rightDiagonalDown *= this->sensorLength;

	FVector leftDiagonalDown = -rightDiagonalDown;
	leftDiagonalDown.Z = rightDiagonalDown.Z;

	//Side-Sensoric
	FVector front = forwardVector * sensorLength;
	FVector back = -forwardVector * sensorLength;
	FVector left = FVector::CrossProduct(forwardVector, zVector) * sensorLength;
	FVector right = FVector::CrossProduct(zVector, forwardVector) * sensorLength;

	//Diagonal-Side-Sensoric
	FVector leftBackDiagonal = (FVector::CrossProduct(forwardVector, zVector) - forwardVector);
	leftBackDiagonal.Normalize();
	leftBackDiagonal *= sensorLength;
	FVector rightFrontDiagonal = leftBackDiagonal * -1;

	FVector rightBackDiagonal = (FVector::CrossProduct(zVector, forwardVector) - forwardVector);
	rightBackDiagonal.Normalize();
	rightBackDiagonal *= sensorLength;
	FVector leftFrontDiagonal = -rightBackDiagonal;

	std::vector<double> sensorData;
	sensorData.push_back(scanSensor(actorLocation, frontDiagonalDown, world));
	sensorData.push_back(scanSensor(actorLocation, backDiagonalDown, world));
	sensorData.push_back(scanSensor(actorLocation, leftDiagonalDown, world));
	sensorData.push_back(scanSensor(actorLocation, rightDiagonalDown, world));

	sensorData.push_back(scanSensor(actorLocation, front, world, true, FColor(0, 255, 0)));
	sensorData.push_back(scanSensor(actorLocation, back, world));
	sensorData.push_back(scanSensor(actorLocation, left, world));
	sensorData.push_back(scanSensor(actorLocation, right, world));

	sensorData.push_back(scanSensor(actorLocation, leftFrontDiagonal, world));
	sensorData.push_back(scanSensor(actorLocation, rightFrontDiagonal, world));
	sensorData.push_back(scanSensor(actorLocation, leftBackDiagonal, world));
	sensorData.push_back(scanSensor(actorLocation, rightBackDiagonal, world));

	return sensorData;
}

double SideAndGroundSensoric::scanSensor(const FVector& from, const FVector& direction,
	UWorld* world, bool drawRay, const FColor& color)
{
	FHitResult hitResult;
	FVector to = from + direction;

	world->LineTraceSingleByChannel(hitResult, from, to, ECC_WorldStatic, collisionParams);
	float hitResultDistance = hitResult.Distance;

	if (!hitResult.bBlockingHit)
		hitResultDistance = sensorLength;
	else
		to = hitResult.Location;

	if (drawRay)
		DrawDebugLine(world, from, to, color, false, -1, 0, 6.333);

	double adjustedData = 1 - hitResultDistance / sensorLength;
	return adjustedData;
}
