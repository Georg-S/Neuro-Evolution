#pragma once
#include <string>
#include "EngineUtils.h"
#include "EngineMinimal.h"

template<typename T>
static void FindAllActors(UWorld* World, TArray<T*>& Out)
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

static std::string convertFstringToStd(const FString& fstr) {
	return TCHAR_TO_UTF8(*fstr);
}

static void printWarningMessageToConsole(const std::string& str) {
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(str.c_str()));
}

static void printDebugMessageOnScreen(const std::string& str, const FColor& color = FColor::Red) {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, color, *FString(str.c_str()));
}