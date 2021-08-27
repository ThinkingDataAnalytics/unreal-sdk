// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

namespace UnrealBuildTool.Rules
{
    public class ThinkingAnalytics : ModuleRules
    {
#if WITH_FORWARDED_MODULE_RULES_CTOR
        public ThinkingAnalytics(ReadOnlyTargetRules Target) : base(Target)
#else
        public ThinkingAnalytics(TargetInfo Target)
#endif
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
            
            PrivateIncludePaths.AddRange(
            new string[] {
                
                // ... add other private include paths required here ...
            }
            );
            
            
            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                // ... add other public dependencies that you statically link with here ...
            }
            );
            PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
            PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
            PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
            
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // ... add private dependencies that you statically link with here ...
                "Core",
                "Analytics",
                "CoreUObject",
                "Json",
                "Engine",
                "Projects"
            }
            );
            
            PrivateIncludePathModuleNames.AddRange(
            new string[]
            {
                "Settings",
                "Projects"
            }
            );
            
            if (Target.Platform == UnrealTargetPlatform.IOS) {
                PublicAdditionalFrameworks.Add(
                    new Framework(
                        "ThinkingSDK",
                        "../ThirdParty/iOS/ThinkingSDK.embeddedframework.zip"
                    )
                );
            }
            else if(Target.Platform == UnrealTargetPlatform.Android)
            {
                // Unreal Plugin Language
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "ThinkingAnalytics_UPL.xml"));
                // JNI
                PublicIncludePathModuleNames.Add("Launch");
            }
        }
    }
}
