// Fill out your copyright notice in the Description page of Project Settings.

#include "SideSensoric.h"

SideSensoric::SideSensoric(FCollisionQueryParams collisionParams)
{
	this->collisionParams = collisionParams;
}

SideSensoric::~SideSensoric()
{
}

std::vector<double> SideSensoric::getDataFromSensors(const FVector &forwardVector, const FVector &actorLocation, UWorld* world)
{
	std::vector<double> sensorData;

	int distance = 200;

	FHitResult hitResult1;
	FVector startVector = actorLocation;
	FVector testVector1 = actorLocation + (forwardVector*distance);
	//	testVector1.Z -= distance;

	FHitResult hitResult2;
	FVector testVector2 = actorLocation + (forwardVector*(-1 * distance));
	//	testVector2.Z -= distance;

	FHitResult hitResult3;
	FVector zVector = FVector(0.f, 0.f, 1.f);
	zVector.Normalize(0.f);
	FVector testVector3 = FVector::CrossProduct(forwardVector, zVector) * distance;
	testVector3 += actorLocation;
	//	testVector3.Z -= distance;

	FHitResult hitResult4;
	FVector testVector4 = FVector::CrossProduct(zVector, forwardVector) * distance;
	testVector4 += actorLocation;
	//	testVector4.Z -= distance;


	world->LineTraceSingleByChannel(hitResult1, startVector, testVector1, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult2, startVector, testVector2, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult3, startVector, testVector3, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult4, startVector, testVector4, ECC_WorldStatic, collisionParams);

	float hitResult1Distance = hitResult1.Distance;
	float hitResult2Distance = hitResult2.Distance;
	float hitResult3Distance = hitResult3.Distance;
	float hitResult4Distance = hitResult4.Distance;
	if (!hitResult1.bBlockingHit) {
		hitResult1Distance = distance;
	}
	else {
		testVector1 = hitResult1.Location;
	}

	if (!hitResult2.bBlockingHit) {
		hitResult2Distance = distance;
	}
	else {
		testVector2 = hitResult2.Location;
	}

	if (!hitResult3.bBlockingHit) {
		hitResult3Distance = distance;
	}
	else {
		testVector3 = hitResult3.Location;
	}

	if (!hitResult4.bBlockingHit) {
		hitResult4Distance = distance;
	}
	else {
		testVector4 = hitResult4.Location;
	}
	double adjustedData1 = 1 - hitResult1Distance / distance;
	double adjustedData2 = 1 - hitResult2Distance / distance;
	double adjustedData3 = 1 - hitResult3Distance / distance;
	double adjustedData4 = 1 - hitResult4Distance / distance;

	DrawDebugLine(
		world,
		startVector,
		testVector1,
		FColor(0, 255, 0),
		false, -1, 0,
		6.333
	);


	DrawDebugLine(
		world,
		startVector,
		testVector2,
		FColor(255, 0, 0),
		false, -1, 0,
		6.333
	);

	DrawDebugLine(
		world,
		startVector,
		testVector3,
		FColor(255, 0, 0),
		false, -1, 0,
		6.333
	);

	DrawDebugLine(
		world,
		startVector,
		testVector4,
		FColor(255, 0, 0),
		false, -1, 0,
		6.333
	);

	sensorData.clear();
	sensorData.push_back(adjustedData1);
	sensorData.push_back(adjustedData2);
	sensorData.push_back(adjustedData3);
	sensorData.push_back(adjustedData4);

	return sensorData;
}
