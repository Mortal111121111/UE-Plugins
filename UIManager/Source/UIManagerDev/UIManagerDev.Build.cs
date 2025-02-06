using UnrealBuildTool;

public class UIManagerDev : ModuleRules
{
	public UIManagerDev(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"BlueprintGraph",
				"UMG",
				"SlateCore",
				"KismetCompiler",
				"GraphEditor",
				"AssetRegistry",
				"StructUtils",
				"UIManager"
			}
		);
	}
}