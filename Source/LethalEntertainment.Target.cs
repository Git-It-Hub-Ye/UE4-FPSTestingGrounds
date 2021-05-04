// Copyright 2018 - 2021 Stuart McDonald.

using UnrealBuildTool;
using System.Collections.Generic;

public class LethalEntertainmentTarget : TargetRules
{
    //public LethalEntertainmentTarget(TargetInfo Target)
    public LethalEntertainmentTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.Add("LethalEntertainment");
	}

	//
	// TargetRules interface.
	//
    /*
	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.Add("LethalEntertainment");
	}
    */
}
