// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SuraS : ModuleRules
{
	public SuraS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AnimGraphRuntime", //Added by Boranaga for Animation
			"AIModule",			// Added by Yoony for AI
			"NavigationSystem",	// Added by Yoony for AI Navigation
            "UMG",			 //Added by Boranaga for the use of UI
            "EngineCameras", //Added by Boranaga for the use of CameraShake
			"Niagara",        //Added by Boranaga for the use of ParticleSystem
			"PhysicsCore", //Added by Boranaga for Animation
			"BoranagaSKControl", //Added by Boranaga for Animation
            "GameplayTags" // Added by <Eren Yeager> for the use of GameplayTags
        });


        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "BlueprintGraph" //Added by Boranaga for Animation
            });
        }

    }
}
