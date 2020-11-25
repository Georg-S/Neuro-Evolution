// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealBachelor : ModuleRules
{
	public UnrealBachelor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        PublicAdditionalLibraries.Add("C:/Users/Georg/Downloads/Unreal_Engine_Project/Source/libraries/NEAT.lib");
        PublicAdditionalLibraries.Add("C:/Users/Georg/Downloads/Unreal_Engine_Project/Source/libraries/HistoricalNeuroEvolution.lib");
    }
}
