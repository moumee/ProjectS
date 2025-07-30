using UnrealBuildTool;

public class BoranagaSKControlEditor : ModuleRules
{
    public BoranagaSKControlEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        OverridePackageType = PackageOverrideType.EngineDeveloper;

        PrivateDependencyModuleNames.AddRange(
            new string[] { 
                "Core", 
                "CoreUObject", 
                "Engine", 
                "BoranagaSKControl", 
                "Slate", 
                "SlateCore",
                "BlueprintGraph",
                "AnimGraph"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "InputCore",
                "EditorFramework",
                "UnrealEd",
                "GraphEditor",
                "PropertyEditor",

                "ContentBrowser",
                "KismetWidgets",
                "ToolMenus",
                "KismetCompiler",
                "Kismet",
                "EditorWidgets",
                "ToolWidgets",
                "AnimationEditMode",
                "DeveloperSettings"
            }
        );

        CircularlyReferencedDependentModules.AddRange(
            new string[] {
                "UnrealEd",
                "GraphEditor",
            }
        );

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "AnimationBlueprintEditor",
            }
        );
    }
}