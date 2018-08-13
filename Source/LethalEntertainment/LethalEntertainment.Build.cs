// Copyright 2018 Stuart McDonald.

using UnrealBuildTool;

public class LethalEntertainment : ModuleRules
{
	//public LethalEntertainment(TargetInfo Target)
    public LethalEntertainment(ReadOnlyTargetRules Target) : base (Target)
    {
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "AIModule", "GameplayTasks" });
	}
}
