// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HFSM2 : ModuleRules
{
	public HFSM2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {});
		PrivateIncludePaths.AddRange(new string[] {});
		PublicDependencyModuleNames.AddRange(new string[] {"Core"});
		PrivateDependencyModuleNames.AddRange(new string[] {});
		DynamicallyLoadedModuleNames.AddRange(new string[] {});
	}
}
