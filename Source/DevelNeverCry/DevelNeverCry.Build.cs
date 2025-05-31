// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DevelNeverCry : ModuleRules
{
	public DevelNeverCry(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTasks", "UMG"});
	}
}
