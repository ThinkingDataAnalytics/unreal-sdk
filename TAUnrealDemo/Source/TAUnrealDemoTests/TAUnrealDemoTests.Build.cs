// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class TAUnrealDemoTests : ModuleRules
{
	public TAUnrealDemoTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Json",
			"JsonUtilities",
			"TDAnalytics"
		});

		// Allow tests to include plugin-private helpers (e.g. Common/TAUtils.h) without publishing them.
		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Plugins/TDAnalytics/Source/TDAnalytics/Private")));
	}
}
