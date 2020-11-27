// Fill out your copyright notice in the Description page of Project Settings.

#include "GroundSensoric.h"

GroundSensoric::GroundSensoric(FCollisionQueryParams collisionParams)
{
	this->collisionParams = collisionParams;
}

std::vector<double> GroundSensoric::getDataFromSensors(const FVector& forwardVector, const FVector& actorLocation, UWorld* world)
{

	int distance = 200;
	FVector zVector = FVector(0.f, 0.f, 1.f);


	FVector frontGround = actorLocation + (forwardVector * distance);
	frontGround.Z -= distance;

	FVector backGround = actorLocation + (-forwardVector * distance);
	backGround.Z -= distance;

	FVector rightGround = FVector::CrossProduct(forwardVector, zVector) * distance;
	rightGround += actorLocation;
	rightGround.Z -= distance;

	FVector leftGround = FVector::CrossProduct(zVector, forwardVector) * distance;
	leftGround += actorLocation;
	leftGround.Z -= distance;

	std::vector<double> sensorData;
	sensorData.push_back(buf(actorLocation, frontGround, world));
	sensorData.push_back(buf(actorLocation, backGround, world));
	sensorData.push_back(buf(actorLocation, rightGround, world));
	sensorData.push_back(buf(actorLocation, leftGround, world));


	return sensorData;
}

double GroundSensoric::buf(const FVector& from, FVector to, UWorld* world)
{
	int sensorLength = 200;

	FHitResult hitResult1;

	world->LineTraceSingleByChannel(hitResult1, from, to, ECC_WorldStatic, collisionParams);
	float hitResult1Distance = hitResult1.Distance;

	if (!hitResult1.bBlockingHit)
		hitResult1Distance = sensorLength;
	else
		to = hitResult1.Location;

	double adjustedData1 = 1 - hitResult1Distance / sensorLength;

	if (this->drawSensor) {
		DrawDebugLine(
			world,
			from,
			to,
			FColor(0, 255, 0),
			false, -1, 0,
			6.333
		);
	}
	return adjustedData1;
}
