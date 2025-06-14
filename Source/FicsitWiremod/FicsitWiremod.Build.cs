using UnrealBuildTool;
using System.IO;
using System;

public class FicsitWiremod : ModuleRules
{
    public FicsitWiremod(ReadOnlyTargetRules Target) : base(Target)
    {
	    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	    CppStandard = CppStandardVersion.Cpp20;

	    // FactoryGame transitive dependencies
	    // Not all of these are required, but including the extra ones saves you from having to add them later.
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "CoreOnline",
            "Engine",
            "DeveloperSettings",
            "PhysicsCore",
            "InputCore",
			"GeometryCollectionEngine",
			"AnimGraphRuntime",
			"AssetRegistry",
            "NavigationSystem",
			"AIModule",
            "GameplayTasks",
            "SlateCore", "Slate", "UMG",
			"RenderCore",
            "CinematicCamera",
            "Foliage",
			"NetCore",
            "GameplayTags",
            "Json", "JsonUtilities",
			"HTTP",
			"EnhancedInput",
			"OnlineSubsystem","OnlineSubsystemUtils","OnlineSubsystemNull","OnlineIntegration"
        });

        // FactoryGame plugins
        PublicDependencyModuleNames.AddRange(new[] {
		    "AbstractInstance",
	    });

	    // Header stubs
	    PublicDependencyModuleNames.AddRange(new[] {
		    "DummyHeaders",
	    });

	    if (Target.Type == TargetRules.TargetType.Editor) {
		    PublicDependencyModuleNames.AddRange(new string[] {"OnlineBlueprintSupport", "AnimGraph", "AssetRegistry"});
	    }
	    PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame", "SML"});
    }
}