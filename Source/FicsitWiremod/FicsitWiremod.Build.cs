using UnrealBuildTool;
using System.IO;
using System;

public class FicsitWiremod : ModuleRules
{
    public FicsitWiremod(ReadOnlyTargetRules Target) : base(Target)
    {
	    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

	    // FactoryGame transitive dependencies
	    // Not all of these are required, but including the extra ones saves you from having to add them later.
	    PublicDependencyModuleNames.AddRange(new[] {
		    "Core", "CoreUObject",
		    "Engine",
		    "DeveloperSettings",
		    "PhysicsCore",
		    "InputCore",
		    "CoreOnline",
		    "GeometryCollectionEngine",
		    "ChaosSolverEngine",
		    "AnimGraphRuntime",
		    "AssetRegistry",
		    "NavigationSystem",
		    "AIModule",
		    "GameplayTasks",
		    "SlateCore", "Slate", "UMG",
		    "RenderCore",
		    "CinematicCamera",
		    "Foliage",
		    "EnhancedInput",
		    "NetCore",
		    "GameplayTags",
		    "HTTP",
		    "AudioMixer",
		    "Json",
		    "JsonUtilities",
		    "Synthesis"
	    });

	    // FactoryGame plugins
	    PublicDependencyModuleNames.AddRange(new[] {
		    "AbstractInstance"
	    });

	    // Header stubs
	    PublicDependencyModuleNames.AddRange(new[] {
		    "DummyHeaders",
	    });

	    if (Target.Type == TargetRules.TargetType.Editor) {
		    PublicDependencyModuleNames.AddRange(new string[] {"OnlineBlueprintSupport", "AnimGraph", "SynthesisEditor", "AssetRegistry"});
	    }
	    PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame", "SML"});
    }
}