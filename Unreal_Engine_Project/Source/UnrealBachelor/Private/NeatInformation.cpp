// Fill out your copyright notice in the Description page of Project Settings.


#include "NeatInformation.h"

ANeatInformation::ANeatInformation()
{
    static ConstructorHelpers::FClassFinder<UMyUserWidget> WidgetAsset(TEXT("/Game/NEAT_Experiment/test_BP"));
    if (WidgetAsset.Succeeded())
    {
        WidgetClass = WidgetAsset.Class;
    }
}

void ANeatInformation::BeginPlay() 
{
    Super::BeginPlay();

    if (WidgetClass)
    {
        Widget = CreateWidget<UMyUserWidget>(GetWorld(), WidgetClass);

        if (Widget)
        {
            Widget->AddToViewport();
        }
    }

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
	DrawText("Current Generation: " + FString::FromInt(generation), FLinearColor::Red, 100, 100, NULL, 1.5f);
}