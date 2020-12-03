// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "Helper.h"
#include "NeatInformation.generated.h"

UCLASS()
class UNREALBACHELOR_API ANeatInformation : public AHUD
{
	GENERATED_BODY()
public:
	ANeatInformation();

	void DrawHUD() override;
	void Tick(float DeltaSeconds) override;

	int generation = -1;

private:
	void drawCurrentGeneration();
};
