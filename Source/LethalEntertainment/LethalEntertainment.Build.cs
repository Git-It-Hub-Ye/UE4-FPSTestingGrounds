// Copyright 2018 - 2020 Stuart McDonald.

using UnrealBuildTool;

public class LethalEntertainment : ModuleRules
{
	//public LethalEntertainment(TargetInfo Target)
    public LethalEntertainment(ReadOnlyTargetRules Target) : base (Target)
    {
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore", "HeadMountedDisplay", "AIModule", "GameplayTasks" });
	}
}
