// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class UnrealBachelor : ModuleRules
{
	public UnrealBachelor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

		string libBinPath = Path.Combine(ModuleDirectory, "libs", "NEAT", "bin");
		string libIncludePath = Path.Combine(ModuleDirectory, "libs", "NEAT", "include");

		PublicAdditionalLibraries.Add(Path.Combine(libBinPath, "NEAT.lib"));
		PublicIncludePaths.Add(Path.Combine(libIncludePath, "NEAT"));
		//PublicAdditionalLibraries.Add("C:/Users/Georg/Downloads/Unreal_Engine_Project/Source/libraries/HistoricalNeuroEvolution.lib");
	}
}