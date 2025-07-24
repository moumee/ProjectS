using UnrealBuildTool;

public class BoranagaSKControlEditor : ModuleRules
{
    public BoranagaSKControlEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BoranagaSKControl", "Slate", "SlateCore" });
    }
}