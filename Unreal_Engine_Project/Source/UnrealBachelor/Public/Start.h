// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Start.generated.h"

UCLASS()
class UNREALBACHELOR_API AStart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStart();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;
};
