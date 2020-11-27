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
	void Tick(float DeltaTime) override;
	void BeginPlay() override;

private:
	void printCurrentGeneration(NEAT* neat);
	double getHighestFitness(const std::vector<double>& fitness) const;
	void initializeMovingObstaclesLevel4();
	void initializeMovingObstaclesLevel5And6();
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
	void checkIfActorsMovedEnough();

	//	NeuralNetwork* neat;
	NEAT* neat;
	TArray<ABoid*> boids;
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
	bool initializeMovingObstaclesLevelFour = false;
	bool initializeMovingObstaclesLevelFiveAndSix = false;
	double highestFitness = 0;
};

