// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GhostStoryV2 : ModuleRules
{
	public GhostStoryV2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
