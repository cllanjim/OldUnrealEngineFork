// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModuleUI : ModuleRules
{
	public ModuleUI(TargetInfo Target)
	{
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
                "InputCore",
				"Projects",
				"Slate",
				"SlateCore",
				"Engine"
			}
		);
	}
}
