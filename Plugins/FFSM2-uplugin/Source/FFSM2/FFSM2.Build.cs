// Created by Andrew Gresyk, see https://flat.hfsm.dev.

using UnrealBuildTool;

public class FFSM2 : ModuleRules
{
	public FFSM2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {});
		PrivateIncludePaths.AddRange(new string[] {});
		PublicDependencyModuleNames.AddRange(new string[] {"Core"});
		PrivateDependencyModuleNames.AddRange(new string[] {});
		DynamicallyLoadedModuleNames.AddRange(new string[] {});
	}
}
