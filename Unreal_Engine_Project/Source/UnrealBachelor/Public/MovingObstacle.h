// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingObstacle.generated.h"

UCLASS()
class UNREALBACHELOR_API AMovingObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void movementStrategy();
	void setMovePoints(FVector movePoint1, FVector movePoint2);
	void setVelocity(float velocity);
	void reset();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

private:
	bool arrive(FVector point);

	FVector initialPosition;
	FVector movePoint1;
	FVector movePoint2;
	float arrivedRadius = 100;
	bool moveToPointOne = true;
	float velocity = 10.f;
};
