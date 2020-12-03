// Fill out your copyright notice in the Description page of Project Settings.


#include "NeatInformation.h"

ANeatInformation::ANeatInformation()
{

}

void ANeatInformation::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
}

void ANeatInformation::DrawHUD() 
{
	Super::DrawHUD();
	drawCurrentGeneration();
}

void ANeatInformation::drawCurrentGeneration() 
{
	int generation = -1;
	if (AMyGameMode* GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode()))
	{
		generation = GameMode->getCurrentGeneration();
	}
	DrawText("Current Generation: " + FString::FromInt(generation), FLinearColor::Red, 100, 100, NULL, 1.5f);
}