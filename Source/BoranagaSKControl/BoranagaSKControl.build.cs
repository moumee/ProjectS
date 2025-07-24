using UnrealBuildTool;

public class BoranagaSKControl : ModuleRules
{
    public BoranagaSKControl(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
    }
}