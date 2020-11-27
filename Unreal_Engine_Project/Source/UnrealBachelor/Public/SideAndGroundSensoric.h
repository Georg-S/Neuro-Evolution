// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "Sensoric.h"

class UNREALBACHELOR_API SideAndGroundSensoric : public Sensoric
{
public:
	SideAndGroundSensoric(FCollisionQueryParams collisionParams);
	~SideAndGroundSensoric();
	std::vector<double> getDataFromSensors(const FVector &forwardVector, const FVector &actorLocation, UWorld* world) override;

private:
	FCollisionQueryParams collisionParams;
};

