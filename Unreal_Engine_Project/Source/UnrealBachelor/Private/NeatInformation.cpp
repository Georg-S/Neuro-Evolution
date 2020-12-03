// Fill out your copyright notice in the Description page of Project Settings.


#include "NeatInformation.h"

ANeatInformation::ANeatInformation()
{
    static ConstructorHelpers::FClassFinder<UNeatWidget> WidgetAsset(TEXT("/Game/NEAT_Experiment/Widget/NeatWidget_BP"));
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
        Widget = CreateWidget<UNeatWidget>(GetWorld(), WidgetClass);
        
        if (Widget)
        {
            Widget->AddToViewport();
        }
    }

}

void ANeatInformation::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
    Widget->currentGeneration = generation;
}