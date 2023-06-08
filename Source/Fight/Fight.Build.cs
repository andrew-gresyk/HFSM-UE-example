// Created by Andrew Gresyk, see https://hfsm.dev.

using UnrealBuildTool;

public class Fight : ModuleRules
{
	public Fight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"FFSM2",
			"HFSM2",
			"Slate",
			"SlateCore"
		});

		//PrivateDependencyModuleNames.AddRange(new string[] {
		//	"OnlineSubsystem",
		//);
	}
}
