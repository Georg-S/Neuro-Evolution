// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingObstacle.h"


// Sets default values
AMovingObstacle::AMovingObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("myMesh");
}

// Called when the game starts or when spawned
void AMovingObstacle::BeginPlay()
{
	Super::BeginPlay();
	initialPosition = GetActorLocation();
}

// Called every frame
void AMovingObstacle::Tick(float DeltaTime)
{
	movementStrategy();
	Super::Tick(DeltaTime);
}

void AMovingObstacle::movementStrategy()
{
	if (moveToPointOne) {
		if (arrive(movePoint1))
			moveToPointOne = !moveToPointOne;
	}
	else {
		if (arrive(movePoint2))
			moveToPointOne = !moveToPointOne;
	}
}

void AMovingObstacle::setMovePoints(FVector movePoint1, FVector movePoint2)
{
	this->movePoint1 = movePoint1 + GetActorLocation();
	this->movePoint2 = movePoint2 + GetActorLocation();
}

void AMovingObstacle::setVelocity(float velocity)
{
	this->velocity = velocity;
}

void AMovingObstacle::reset()
{
	SetActorLocation(initialPosition);
	moveToPointOne = true;
}

bool AMovingObstacle::arrive(FVector point)
{
	FVector newVelocity = point - GetActorLocation();
	newVelocity.Normalize();
	newVelocity *= velocity;
	mesh->SetPhysicsLinearVelocity(newVelocity, false);

	if ((GetActorLocation() - point).Size() <= arrivedRadius)
		return true;

	return false;
}
