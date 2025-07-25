using UnrealBuildTool;

public class BoranagaSKControl : ModuleRules
{
    public BoranagaSKControl(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(
            new string[] { 
                "Core", 
                "CoreUObject", 
                "Engine",
                "AnimationCore"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "TraceLog",
            }
        );

        SetupModulePhysicsSupport(Target);

        // External users of this library do not need to know about Eigen.
        AddEngineThirdPartyPrivateStaticDependencies(Target,
                "Eigen"
                );

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "GeometryCollectionEngine",
            }
        );
    }
}