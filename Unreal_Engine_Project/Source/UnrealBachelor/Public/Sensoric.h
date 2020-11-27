// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "CoreMinimal.h"

class UNREALBACHELOR_API Sensoric
{
public:
	virtual std::vector<double> getDataFromSensors(const FVector &forwardVector, const FVector &actorLocation, UWorld* world) = 0;
};
