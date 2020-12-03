// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"

AMyGameMode::AMyGameMode() {
	HUDClass = ANeatInformation::StaticClass();
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AMyGameMode::BeginPlay()
{
	const int populationSize = 50;
	const int sensorCount = 12;
	const int possibleActions = 7;


	neat = new nev::NEAT(populationSize, sensorCount, possibleActions);

	Super::BeginPlay();
	initializeStartAndGoal();
	FindAllActors(GetWorld(), boids);

	FString levelName = GetWorld()->GetMapName();
	if (levelName.Contains("FourthLevel"))
		initializeMovingObstaclesLevelFour = true;
	else if (levelName.Contains("SecondLevel"))
		distanceBoidMustChange = 1000;
	else
		initializeMovingObstaclesLevelFiveAndSix = true;

	initializeMovingObstaclesLevel4();
	initializeMovingObstaclesLevel5And6();
	printCurrentGeneration(neat);
}

void AMyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (start == nullptr || goal == nullptr)
		return;

	if (handleKeyboardInput())
		return;

	if (solutionFound) {
		if (!writeOnce) {
			nev::FileWriter::writeNEATToFile(*neat, getSaveString());
			solutionFound = false;
		}
	}

	numberTicks++;
	checkIfActorsMovedEnough();
	if (atleastOneActive())
		updateActors();
	else {
		std::vector<double> fitness = calculateFitnessOfAllActors();
		neat->iterateOneGeneration(fitness);
		resetActors();
		resetMovingObstacles();
		printCurrentGeneration(neat);
	}
}

int AMyGameMode::getCurrentGeneration()
{
	if (this->neat != nullptr)
		return neat->getCurrentGeneration();
	
	return -1;
}

void AMyGameMode::printCurrentGeneration(nev::NEAT* neat)
{
	printWarningMessageToConsole("Current Generation " + std::to_string(neat->getCurrentGeneration()));
}

double AMyGameMode::getHighestFitness(const std::vector<double>& fitness) const
{
	return *(std::max_element(fitness.begin(), fitness.end()));
}

void AMyGameMode::initializeMovingObstaclesLevel4()
{
	if (!initializeMovingObstaclesLevelFour)
		return;

	FindAllActors(GetWorld(), movingObstacles);
	float movingObstacleVelocity = 400;
	for (int i = 0; i < movingObstacles.Num(); i++) {
		movingObstacles[i]->setMovePoints(FVector(0, 900, 0), FVector(0, -900, 0));
		movingObstacles[i]->setVelocity(movingObstacleVelocity);
		movingObstacleVelocity += 50;
	}
}

void AMyGameMode::initializeMovingObstaclesLevel5And6()
{
	if (!initializeMovingObstaclesLevelFiveAndSix)
		return;

	FindAllActors(GetWorld(), movingObstacles);
	float movingObstacleVelocity = 400;
	for (int i = 0; i < movingObstacles.Num(); i++) {
		movingObstacles[i]->setMovePoints(FVector(0, 100, 0), FVector(0, -600, 0));
		movingObstacles[i]->setVelocity(movingObstacleVelocity);
	}
}

void AMyGameMode::resetMovingObstacles()
{
	for (int i = 0; i < movingObstacles.Num(); i++)
		movingObstacles[i]->reset();
}

bool AMyGameMode::updateActive(ABoid* boid)
{
	if (boid->GetActorLocation().Z < inactiveZthreshold) {
		boid->deactivate();
		return false;
	}
	if (((boid->GetActorLocation() - goal->GetActorLocation()).Size()) <= goalReachedThreshhold) {
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

	if (starts.Num() >= 1)
		start = starts[0];

	if (goals.Num() >= 1)
		goal = goals[0];
}

void AMyGameMode::updateActors()
{
	for (int i = 0; i < boids.Num(); i++) {
		updateActive(boids[i]);
		if (!boids[i]->getActive())
			continue;

		boids[i]->updateSensorData();
		std::vector<double> output = neat->calculateOutputActiveOfSpecificGenotype(boids[i]->getSensorData(), i);
		boids[i]->updateMoveDirection(output);
		boids[i]->iterate();
	}
}

bool AMyGameMode::atleastOneActive()
{
	for (int i = 0; i < boids.Num(); i++) {
		if (boids[i]->getActive())
			return true;
	}
	return false;
}

bool AMyGameMode::handleKeyboardInput()
{
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::One)) {
		delete neat;
		UGameplayStatics::OpenLevel(GetWorld(), "FirstLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Two)) {
		delete neat;
		UGameplayStatics::OpenLevel(GetWorld(), "SecondLevel");
		distanceBoidMustChange = 1000;
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Three)) {
		delete neat;
		UGameplayStatics::OpenLevel(GetWorld(), "ThirdLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Four)) {
		delete neat;
		UGameplayStatics::OpenLevel(GetWorld(), "FourthLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five)) {
		delete neat;
		UGameplayStatics::OpenLevel(GetWorld(), "FifthLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six)) {
		delete neat;
		UGameplayStatics::OpenLevel(GetWorld(), "SixthLevel");
		return true;
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L)) {
		delete neat;
		loadNEATFromFile();
		resetActors();
		resetMovingObstacles();
	}
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R)) {
		resetActors();
		resetMovingObstacles();
	}

	return false;
}


std::vector<double> AMyGameMode::calculateFitnessOfAllActors()
{
	std::vector<double> fitness;
	bool printOnce = false;

	for (int i = 0; i < boids.Num(); i++) {
		fitness.push_back(calculateFitnessOfActor(boids[i]));

		if (solutionFound && (!printOnce)) {
			printOnce = true;
			printWarningMessageToConsole("Solved in Generation " + std::to_string(neat->getCurrentGeneration()));
			printWarningMessageToConsole("Hidden Node Count " + std::to_string(neat->getHiddenNeuronCountOfSpecificGenotype(i)));
			printWarningMessageToConsole("Enabled Links " + std::to_string(neat->getCountOfEnabledLinksOfSpecificGenotype(i)));
		}
	}
	return fitness;
}

double AMyGameMode::calculateFitnessOfActor(ABoid* actor)
{
	double fitness = 0;
	FVector actorLocation = actor->GetActorLocation();
	double startLength = (start->GetActorLocation() - goal->GetActorLocation()).Size();
	double actorDistanceFromGoal = (actorLocation - goal->GetActorLocation()).Size();

	if (actorDistanceFromGoal > startLength)
		return 0.f;
	else
		fitness = startLength - actorDistanceFromGoal;

	if (actorDistanceFromGoal <= goalReachedThreshhold) {
		fitness = startLength;
		solutionFound = true;
	}

	return fitness;
}

void AMyGameMode::resetActors()
{
	for (int i = 0; i < boids.Num(); i++) {
		boids[i]->resetVelocity();
		FVector startPosition = start->GetActorLocation();
		boids[i]->SetActorLocationAndRotation(startPosition, FRotator(0.f, 0.f, 0.f));
		boids[i]->activate();
	}
}

void AMyGameMode::checkIfActorsMovedEnough()
{
	if (numberTicks == numberTicksPositionMustChange) {
		numberTicks = 0;

		for (auto boid : boids) {
			if ((boid->GetActorLocation() - boid->getPastPosition()).Size() < distanceBoidMustChange) {
				boid->deactivate();
			}
			boid->setPastPosition(boid->GetActorLocation());
		}
	}
}

void AMyGameMode::loadNEATFromFile()
{
	std::string populationString = getLoadString() + "population.txt";
	std::string innovationString = getLoadString() + "innovation.txt";
	std::string neatString = getLoadString() + "neat.txt";

	neat = nev::FileReader::getNewNEATFromFiles(neatString, populationString, innovationString);
}

std::string AMyGameMode::getRootDir() const
{
	return convertFstringToStd(FPaths::ProjectDir());
}

std::string AMyGameMode::getSaveString() const
{
	return getRootDir() + "/NeuralNetworks/Current/" + getLevelName() + "_";
}

std::string AMyGameMode::getLoadString() const
{
	return getRootDir() + "/NeuralNetworks/Saved/" + getLevelName() + "_";
}

std::string AMyGameMode::getLevelName() const
{
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	return convertFstringToStd(LevelName);
}