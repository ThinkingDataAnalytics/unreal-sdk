// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class TAUnrealDemo : ModuleRules
{
	public TAUnrealDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "TDAnalytics", "Json", "JsonUtilities"});

		PrivateDependencyModuleNames.AddRange(new string[] { "TDAnalytics", "Json", "JsonUtilities"});

		bool bEnableAndroidTests = Target.Platform == UnrealTargetPlatform.Android
			&& (Target.Configuration == UnrealTargetConfiguration.Debug
				|| Target.Configuration == UnrealTargetConfiguration.DebugGame
				|| Target.Configuration == UnrealTargetConfiguration.Development)
			&& !Target.bForceDisableAutomationTests;
		PrivateDefinitions.Add("TDANALYTICS_ANDROID_TESTS=" + (bEnableAndroidTests ? "1" : "0"));

		if (bEnableAndroidTests)
		{
			PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Plugins/TDAnalytics/Source/TDAnalytics/Private")));
			PrivateIncludePathModuleNames.Add("Launch");
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
