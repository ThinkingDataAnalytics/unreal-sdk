// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

namespace UnrealBuildTool.Rules
{
    public class TDAnalytics : ModuleRules
    {
        private void CopyToBinaries(string Filepath, /*TargetInfo*/ReadOnlyTargetRules Target)
        {
            string binariesDir = Path.Combine(ModuleDirectory, "../../../../Binaries/", Target.Platform.ToString());
            string filename = Path.GetFileName(Filepath);

            if (!Directory.Exists(binariesDir))
                Directory.CreateDirectory(binariesDir);

            if (!File.Exists(Path.Combine(binariesDir, filename)))
                File.Copy(Filepath, Path.Combine(binariesDir, filename), true);
        }

#if WITH_FORWARDED_MODULE_RULES_CTOR
        public TDAnalytics(ReadOnlyTargetRules Target) : base(Target)
#else
        public TDAnalytics(TargetInfo Target)
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
                // "SlateCore",
                "CoreUObject",
                "Engine",
                "HTTP",
                "Json",
                "JsonUtilities",
                "Sockets",
                "EngineSettings"
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
                // "SlateCore",
                "Analytics",
                "CoreUObject",
                "Json",
                "Engine",
                "Projects",
                "HTTP",
                "Json",
                "JsonUtilities",
                "Sockets",
                "EngineSettings"
            }
            );
            if (Target.Type == TargetRules.TargetType.Editor)
            {
                PrivateDependencyModuleNames.Add("UnrealEd");
            }
            
            PrivateIncludePathModuleNames.AddRange(
            new string[]
            {
                "Settings",
                "Projects"
            }
            );

            if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                PublicAdditionalFrameworks.Add(
                    new Framework(
                        "ThinkingSDK",
                        "../ThirdParty/iOS/ThinkingSDK.embeddedframework.zip"
                    )
                );

                PublicAdditionalFrameworks.Add(
                    new Framework(
                        "ThinkingDataCore",
                        "../ThirdParty/iOS/ThinkingDataCore.embeddedframework.zip"
                    )
                );
            }
            else if (Target.Platform == UnrealTargetPlatform.Android)
            {
                // Unreal Plugin Language
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "TDAnalytics_UPL.xml"));
                // JNI
                PublicIncludePathModuleNames.Add("Launch");
            }
            else if (Target.Platform == UnrealTargetPlatform.Win64) {
                CopyToBinaries(Path.Combine(ModuleDirectory, "../ThirdParty/Windows", "Win64", "thinkingdata.dll"), Target);
                CopyToBinaries(Path.Combine(ModuleDirectory, "../ThirdParty/Windows", "Win64", "sqlite3.dll"), Target);
                CopyToBinaries(Path.Combine(ModuleDirectory, "../ThirdParty/Windows", "Win64", "zlibwapi.dll"), Target);
                CopyToBinaries(Path.Combine(ModuleDirectory, "../ThirdParty/Windows", "Win64", "libcrypto-3-x64.dll"), Target);
                CopyToBinaries(Path.Combine(ModuleDirectory, "../ThirdParty/Windows", "Win64", "libcurl.dll"), Target);
                CopyToBinaries(Path.Combine(ModuleDirectory, "../ThirdParty/Windows", "Win64", "libssl-3-x64.dll"), Target);
                CopyToBinaries(Path.Combine(ModuleDirectory, "../ThirdParty/Windows", "Win64", "CppWrapper.dll"), Target);
                RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "thinkingdata.dll"));
                RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "sqlite3.dll"));
                RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "zlibwapi.dll"));
                RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "libcrypto-3-x64.dll"));
                RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "libcurl.dll"));
                RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "libssl-3-x64.dll"));
                RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "CppWrapper.dll"));
            }
        }
    }
}
