// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class OGA_GameJam_2023EditorTarget : TargetRules
{
	public OGA_GameJam_2023EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
        

        ExtraModuleNames.AddRange( new string[] { "OGA_GameJam_2023" } );
	}
}
