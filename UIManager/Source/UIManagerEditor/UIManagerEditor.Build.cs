using UnrealBuildTool;

public class UIManagerEditor : ModuleRules
{
	public UIManagerEditor(ReadOnlyTargetRules Target) : base(Target)
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
				"UMG",
				"AssetTools",
				"SlateCore",
				"GraphEditor",
				"AssetRegistry",
				"UMGEditor",
				"UIManager"
			}
		);
	}
}