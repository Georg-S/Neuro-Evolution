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

		string libBinPath = Path.Combine(ModuleDirectory, "libs", "NeuroEvolution", "bin");
		string libIncludePath = Path.Combine(ModuleDirectory, "libs", "NeuroEvolution", "include");

		PublicAdditionalLibraries.Add(Path.Combine(libBinPath, "NeuroEvolution.lib"));
		PublicIncludePaths.Add(libIncludePath);
	}
}