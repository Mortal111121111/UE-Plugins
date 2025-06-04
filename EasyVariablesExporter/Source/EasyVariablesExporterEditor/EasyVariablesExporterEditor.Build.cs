using UnrealBuildTool;

public class EasyVariablesExporterEditor : ModuleRules
{
    public EasyVariablesExporterEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "InputCore",
                "Slate",
                "SlateCore",
                "Engine",
                "BlueprintGraph",
                "EasyVariablesExporter",
                "ApplicationCore",
                "PropertyEditor",
                "GraphEditor",
                "UnrealEd",
                "UMG",
                "Kismet"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
               
            }
        );
    }
}