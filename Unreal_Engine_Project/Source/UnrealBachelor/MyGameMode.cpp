// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"

AMyGameMode::AMyGameMode() {
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AMyGameMode::BeginPlay()
{
	net = new NEAT(50, 12, 7);
//	net = new NEAT("C://Users/Georg/Documents/Studium/8.Semester/Bachelorarbeit/UnrealBachelor/WorkingNeuralNets/Level7Population.txt"
//		, "C://Users/Georg/Documents/Studium/8.Semester/Bachelorarbeit/UnrealBachelor/WorkingNeuralNets/Level7Innovation.txt");

	Super::BeginPlay();

	initializeStartAndGoal();
	FindAllActors(GetWorld(), actors);

	FString test = GetWorld()->GetMapName();
	if (test.Contains("FourthLevel")) {
		initializeMovingObstaclesLevelFour = false;
	}
	else if (test.Contains("SecondLevel")) {
		distanceBoidMustChange = 1000;
	}
	else {
		initializeMovingObstaclesLevelFive = false;
	}


	std::string bli = std::to_string(net->getCurrentGeneration());
	FString blitz(bli.c_str());
	UE_LOG(LogTemp, Warning, TEXT("Current Generation %s"), *blitz);
}

void AMyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (start == nullptr || goal == nullptr)
		return;

	initializeMovingObstaclesLevel4();
	initializeMovingObstaclesLevel5();

	if (checkLevelSwitched()) {
		return;
	}

	/*
	if (solutionFound) {
		if (!writeOnce) {
			net->writePopulationAndInnovationAsFiles("C://Users/Georg/Documents/Studium/8.Semester/Bachelorarbeit/UnrealBachelor/CreatedNetwork/Level7Population.txt"
				, "C://Users/Georg/Documents/Studium/8.Semester/Bachelorarbeit/UnrealBachelor/CreatedNetwork/Level7Innovation.txt");
			writeOnce = true;
		}
		else
			return;
	}
	*/

	numberTicks++;
	checkIfActorsMoved();
	if (atleastOneActive()) {
			updateActors();
	}
	else {
		vector<double> fitness = calculateFitnessOfAllActors();
		net->iterateOneGeneration(fitness);
		resetActors();
		resetMovingObstacles();
		std::string bli = std::to_string(net->getCurrentGeneration());
		FString blitz(bli.c_str());
		UE_LOG(LogTemp, Warning, TEXT("Current Generation %s"), *blitz);
	}
}

double AMyGameMode::getHighestFitness(vector<double> fitness)
{
	double highestFitness = 0;
	for (int i = 0; i < fitness.size(); i++) {
		if (fitness[i] > highestFitness) {
			highestFitness = fitness[i];
		}
	}
	return highestFitness;
}

void AMyGameMode::initializeMovingObstaclesLevel4()
{
	if (initializeMovingObstaclesLevelFour)
		return;

	FindAllActors(GetWorld(), movingObstacles);
	float movingObstacleVelocity = 400;
	for (int i = 0; i < movingObstacles.Num(); i++) {
		movingObstacles[i]->setMovePoints(FVector(0, 900, 0), FVector(0, -900, 0));
		movingObstacles[i]->setVelocity(movingObstacleVelocity);
		movingObstacleVelocity+= 50;
		initializeMovingObstaclesLevelFour = true;
	}
}

void AMyGameMode::initializeMovingObstaclesLevel5()
{
	if (initializeMovingObstaclesLevelFive)
		return;

	FindAllActors(GetWorld(), movingObstacles);
	float movingObstacleVelocity = 400;
	for (int i = 0; i < movingObstacles.Num(); i++) {
		movingObstacles[i]->setMovePoints(FVector(0, 100, 0), FVector(0, -600, 0));
		movingObstacles[i]->setVelocity(movingObstacleVelocity);
		initializeMovingObstaclesLevelFive = true;
	}
}

void AMyGameMode::resetMovingObstacles()
{
	for (int i = 0; i < movingObstacles.Num(); i++) {
		movingObstacles[i]->reset();
	}
}

bool AMyGameMode::updateActive(ABoid * boid)
{
	if (boid->GetActorLocation().Z < inactiveZthreshold) {
		boid->deactivate();
		return false;
	}
	if (((boid->GetActorLocation() - goal->GetActorLocation()).Size()) < goalReachedThreshhold) {
		boid->deactivate();
		return false;
	}
	return true;
}

void AMyGameMode::initializeStartAndGoal()
{
	TArray<AStart*> starts;
	TArray<AGoal*> goals;
	FindAllActors(GetWorld(), starts);
	FindAllActors(GetWorld(), goals);

	if (starts.Num() >= 1) {
		start = starts[0];
	}
	if (goals.Num() >= 1) {
		goal = goals[0];
	}
}

void AMyGameMode::updateActors()
{
	for (int i = 0; i < actors.Num(); i++) {
		if (!actors[i]->getActive()) {
			continue;
		}
		updateActive(actors[i]);
		if (!actors[i]->getActive())
			continue;

		actors[i]->updateSensorData();
		vector<double> output = net->calculateOutputActiveOfSpecificGenotype(actors[i]->getSensorData(),i);
		actors[i]->updateMoveDirection(output);
		actors[i]->iterate();
	}
}

bool AMyGameMode::atleastOneActive()
{
	for (int i = 0; i < actors.Num(); i++) {
		if (actors[i]->getActive())
			return true;
	}
	return false;
}

bool AMyGameMode::checkLevelSwitched()
{
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One)) {
		delete net;
		UGameplayStatics::OpenLevel(GetWorld(),"FirstLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two)) {
		delete net;
		UGameplayStatics::OpenLevel(GetWorld(), "SecondLevel");
		distanceBoidMustChange = 1000;
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three)) {
		delete net;
		UGameplayStatics::OpenLevel(GetWorld(), "ThirdLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four)) {
		delete net;
		UGameplayStatics::OpenLevel(GetWorld(), "FourthLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five)) {
		delete net;
		UGameplayStatics::OpenLevel(GetWorld(), "FifthLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six)) {
		delete net;
		UGameplayStatics::OpenLevel(GetWorld(), "SixthLevel");
		return true;
	}
	return false;
}


vector<double> AMyGameMode::calculateFitnessOfAllActors()
{
	vector<double> fitness;
	bool printOnce = false;

	for (int i = 0; i < actors.Num(); i++) {
		fitness.push_back(calculateFitnessOfActor(actors[i]));

		if (solutionFound && (!printOnce)) {
			printOnce = true;
			std::string bufStd = std::to_string(net->getCurrentGeneration());
			FString bufFStr(bufStd.c_str());
			UE_LOG(LogTemp, Warning, TEXT("Solved in Generation %s"), *bufFStr);

			bufStd = std::to_string(net->getHiddenNeuronCountOfSpecificGenotype(i));
			bufFStr = bufStd.c_str();
			UE_LOG(LogTemp, Warning, TEXT("Hidden Node Count %s"), *bufFStr);

			bufStd = std::to_string(net->getCountOfEnabledLinksOfSpecificGenotype(i));
			bufFStr = bufStd.c_str();
			UE_LOG(LogTemp, Warning, TEXT("Enabled Links %s"), *bufFStr);
		}
	}
	return fitness;
}

double AMyGameMode::calculateFitnessOfActor(ABoid * actor)
{
	double fitness = 0;
	FVector actorLocation = actor->GetActorLocation();
	double startLength = (start->GetActorLocation() - goal->GetActorLocation()).Size();
	double actorDistanceFromGoal = (actorLocation - goal->GetActorLocation()).Size();

	if (actorDistanceFromGoal > startLength) {
		return 0.f;
	}
	else {
		fitness = startLength - actorDistanceFromGoal;
	}
	if (actorDistanceFromGoal < goalReachedThreshhold) {
		fitness = startLength;
		solutionFound = true;
	}

	return fitness;
}

void AMyGameMode::resetActors()
{

	for (int i = 0; i < actors.Num(); i++) {
		actors[i]->resetVelocity();
		FVector startPosition = start->GetActorLocation();
		actors[i]->SetActorLocationAndRotation(startPosition, FRotator(0.f,0.f,0.f), false, 0, ETeleportType::None);
		actors[i]->activate();
	}
}

void AMyGameMode::checkIfActorsMoved()
{
	if (numberTicks == numberTicksPositionMustChange) {
		numberTicks = 0;

		for (int i = 0; i < actors.Num(); i++) {
			if ((actors[i]->GetActorLocation() - actors[i]->getPastPosition()).Size() <= distanceBoidMustChange) {
				actors[i]->deactivate();
			}
			actors[i]->setPastPosition(actors[i]->GetActorLocation());
		}
	}
}
