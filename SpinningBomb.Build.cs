// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SpinningBomb : ModuleRules
{
	public SpinningBomb(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "SpinningBomb" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",  "EnhancedInput" });

        PublicDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "Steamworks",
            "OnlineSubsystemNull"
        });

        DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemSteam" });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });


        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
