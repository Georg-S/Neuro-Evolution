// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "Sensoric.h"


class UNREALBACHELOR_API GroundSensoric : public Sensoric
{
public:
	GroundSensoric(FCollisionQueryParams collisionParams);
	~GroundSensoric();
	vector<double> getDataFromSensors(const FVector &forwardVector, const FVector &actorLocation, UWorld* world) override;


private:
	FCollisionQueryParams collisionParams;
};
