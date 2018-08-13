// Copyright 2018 Stuart McDonald.

using UnrealBuildTool;
using System.Collections.Generic;

public class LethalEntertainmentEditorTarget : TargetRules
{
    //public LethalEntertainmentEditorTarget(TargetInfo Target)
    public LethalEntertainmentEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
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
