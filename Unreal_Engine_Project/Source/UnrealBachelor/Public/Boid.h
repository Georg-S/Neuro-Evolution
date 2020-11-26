// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <string>
#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Sensoric.h"
#include "SideSensoric.h"
#include "GroundSensoric.h"
#include "Obstacle.h"
#include "SideAndGroundSensoric.h"
#include "Boid.generated.h"



UCLASS()
class UNREALBACHELOR_API ABoid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoid();

	// Called every frame
	void Tick(float DeltaTime) override;
	void resetVelocity();
	void activate();
	void deactivate();
	void setPastPosition(FVector position);
	FVector getPastPosition();
	bool getActive();
	void setId(int id);
	void iterate();
	void updateSensorData();
	vector <double> getSensorData();
	void updateMoveDirection(vector<double> neuralNetOutput);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* collisionSphere;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent*
			OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	float sphereRadius;
	float moveFactor = 0.f;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

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

private:
	FCollisionQueryParams collisionParams;
	void initializeCollisionParams();
	void move();
	void checkIfActive();
	bool active = true;
	int id = 0;
	vector<double> sensorDataForNeuralNet;
	FVector pastPosition = FVector(0, 0, 0);
	Sensoric* sensoric;
	bool isInAir = false;

	bool forward = false;
	bool backwards = false;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool rotateClockwise = false;
	bool rotateCounterClockwise = false;
	static const bool drawDebugSphere = false;
	static const int velocity = 1500;
	static const int jumpVelocity = 750;
};
