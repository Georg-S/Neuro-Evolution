// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <NEAT.h>
#include <string>
#include "Helper.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"
#include "Boid.h"
#include "MovingObstacle.h"
#include "Public/Goal.h"
#include "Public/Start.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

UCLASS()
class UNREALBACHELOR_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	double getHighestFitness(vector<double> fitness);
	void initializeMovingObstaclesLevel4();
	void initializeMovingObstaclesLevel5();
	void resetMovingObstacles();
	bool updateActive(ABoid* boid);
	void initializeStartAndGoal();
	bool atleastOneActive();
	bool checkLevelSwitched();
	UFUNCTION()
		void updateActors();
	vector<double> calculateFitnessOfAllActors();
	double calculateFitnessOfActor(ABoid* actor);
	void resetActors();
	void checkIfActorsMoved();

	//	NeuralNetwork* net;
	NEAT* net;
	TArray<ABoid*> actors;
	TArray<AMovingObstacle*> movingObstacles;
	int numberTicksPositionMustChange = 2300;
	float distanceBoidMustChange = 3000;
	int numberTicks = 0;
	AStart* start = nullptr;
	AGoal* goal = nullptr;
	float goalReachedThreshhold = 1100.f;
	float inactiveZthreshold = -150.f;
	bool solutionFound = false;
	bool writeOnce = false;
	bool initializeMovingObstaclesLevelFour = true;
	bool initializeMovingObstaclesLevelFive = true;
	double highestFitness = 0;
};

