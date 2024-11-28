using UnrealBuildTool;

public class UIExtensionEditor : ModuleRules
{
    public UIExtensionEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Engine",
                "Slate",
                "SlateCore",
                "PropertyEditor",
                "DeveloperSettings",
                "GraphEditor",
                "UIExtension",
                "SlateCore",
                "UnrealEd",
                "AssetRegistry"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}