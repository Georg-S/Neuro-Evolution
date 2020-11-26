// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "Sensoric.h"

using namespace std;

/**
 * 
 */
class UNREALBACHELOR_API SideSensoric : public Sensoric
{
public:
	SideSensoric(FCollisionQueryParams collisionParams);
	~SideSensoric();
	vector<double> getDataFromSensors(const FVector &forwardVector, const FVector &actorLocation, UWorld* world) override;
private:
	FCollisionQueryParams collisionParams;
};
