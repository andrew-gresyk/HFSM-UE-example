// Created by Andrew Gresyk, see https://hfsm.dev.

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
