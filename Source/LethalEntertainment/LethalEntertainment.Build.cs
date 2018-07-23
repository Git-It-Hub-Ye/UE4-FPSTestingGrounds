// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LethalEntertainment : ModuleRules
{
	//public LethalEntertainment(TargetInfo Target)
    public LethalEntertainment(ReadOnlyTargetRules Target) : base (Target)
    {
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "AIModule", "GameplayTasks" });
	}
}
