// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>

using namespace std;
/**
 * 
 */
class UNREALBACHELOR_API Sensoric
{
public:
	Sensoric();
	~Sensoric();
	virtual vector<double> getDataFromSensors(const FVector &forwardVector, const FVector &actorLocation, UWorld* world) = 0;

	template<typename T>
	void FindAllActors(UWorld* World, TArray<T*>& Out)
	{
		for (TActorIterator<AActor> It(World, T::StaticClass()); It; ++It)
		{
			T* Actor = Cast<T>(*It);
			if (Actor && !Actor->IsPendingKill())
			{
				Out.Add(Actor);
			}
		}
	}
};
