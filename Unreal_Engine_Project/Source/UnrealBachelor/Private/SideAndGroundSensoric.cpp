// Fill out your copyright notice in the Description page of Project Settings.

#include "SideAndGroundSensoric.h"

SideAndGroundSensoric::SideAndGroundSensoric(FCollisionQueryParams collisionParams)
{
	this->collisionParams = collisionParams;
}

SideAndGroundSensoric::~SideAndGroundSensoric()
{
}

std::vector<double> SideAndGroundSensoric::getDataFromSensors(const FVector &forwardVector, const FVector &actorLocation, UWorld* world)
{
	std::vector<double> sensorData;

	int distance = 200;
	FVector startVector = actorLocation;
	FVector zVector = FVector(0.f, 0.f, 1.f);
	zVector.Normalize(0.f);

	//Ground-Sensoric
	FHitResult hitResult1;
	FVector testVector1 = actorLocation + (forwardVector*distance);
	testVector1.Z -= distance;

	FHitResult hitResult2;
	FVector testVector2 = actorLocation + (forwardVector*(-1 * distance));
	testVector2.Z -= distance;

	FHitResult hitResult3;
	FVector testVector3 = FVector::CrossProduct(forwardVector, zVector) * distance;
	testVector3 += actorLocation;
	testVector3.Z -= distance;

	FHitResult hitResult4;
	FVector testVector4 = FVector::CrossProduct(zVector, forwardVector) * distance;
	testVector4 += actorLocation;
	testVector4.Z -= distance;

	//Side-Sensoric
	FHitResult hitResult5;
	FVector testVector5 = actorLocation + (forwardVector*distance);

	FHitResult hitResult6;
	FVector testVector6 = actorLocation + (forwardVector*(-1 * distance));

	FHitResult hitResult7;
	FVector testVector7 = FVector::CrossProduct(forwardVector, zVector) * distance;
	testVector7 += actorLocation;

	FHitResult hitResult8;
	FVector testVector8 = FVector::CrossProduct(zVector, forwardVector) * distance;
	testVector8 += actorLocation;

	//Diagonal-Side-Sensoric
	FHitResult hitResult9;
	FVector testVector9 = ((forwardVector*-1) + FVector::CrossProduct(forwardVector, zVector));
	testVector9.Normalize();
	testVector9 *= distance;
	FVector testVector11 = testVector9 * -1;
	testVector9 += actorLocation;

	
	FHitResult hitResult10;
	FVector testVector10 = ((forwardVector*-1) + FVector::CrossProduct(zVector, forwardVector));
	testVector10.Normalize();
	testVector10 *= distance;
	FVector testVector12 = testVector10 * -1;
	testVector10 += actorLocation;

	FHitResult hitResult11;
	testVector11 += actorLocation;
	FHitResult hitResult12;
	testVector12 += actorLocation;


	std::vector<FHitResult*> hitResults;
	hitResults.push_back(&hitResult1);
	hitResults.push_back(&hitResult2);	
	hitResults.push_back(&hitResult3);
	hitResults.push_back(&hitResult4);
	hitResults.push_back(&hitResult5);
	hitResults.push_back(&hitResult6);
	hitResults.push_back(&hitResult7);
	hitResults.push_back(&hitResult8);
	hitResults.push_back(&hitResult9);
	hitResults.push_back(&hitResult10);
	hitResults.push_back(&hitResult11);
	hitResults.push_back(&hitResult12);


	world->LineTraceSingleByChannel(hitResult1, startVector, testVector1, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult2, startVector, testVector2, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult3, startVector, testVector3, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult4, startVector, testVector4, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult5, startVector, testVector5, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult6, startVector, testVector6, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult7, startVector, testVector7, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult8, startVector, testVector8, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult9, startVector, testVector9, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult10, startVector, testVector10, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult11, startVector, testVector11, ECC_WorldStatic, collisionParams);
	world->LineTraceSingleByChannel(hitResult12, startVector, testVector12, ECC_WorldStatic, collisionParams);



	std::vector<FVector*> testVectors;
	testVectors.push_back(&testVector1);
	testVectors.push_back(&testVector2);
	testVectors.push_back(&testVector3);
	testVectors.push_back(&testVector4);
	testVectors.push_back(&testVector5);
	testVectors.push_back(&testVector6);
	testVectors.push_back(&testVector7);
	testVectors.push_back(&testVector8);
	testVectors.push_back(&testVector9);
	testVectors.push_back(&testVector10);
	testVectors.push_back(&testVector11);
	testVectors.push_back(&testVector12);

	sensorData.clear();

	for (int i = 0; i < hitResults.size(); i++) {
		float hitResultDistance = hitResults[i]->Distance;

		if (!hitResults[i]->bBlockingHit) {
			hitResultDistance = distance;
		}
		else {
			testVectors[i] = &hitResults[i]->Location;
		}

		double adjustedData = 1 - hitResultDistance / distance;
		sensorData.push_back(adjustedData);
	}

	for (int i = 0; i < testVectors.size(); i++) {
		FColor color = FColor(255, 0, 0);
		if (i == 4) {
			color = FColor(0, 255, 0);
		}
		DrawDebugLine(
			world,
			startVector,
			*testVectors[i],
			color,
			false, -1, 0,
			6.333
		);
	}
	return sensorData;
}
