// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class VKEditor : ModuleRules
    {
        public VKEditor(TargetInfo Target)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "RenderCore",
                    "RHI",
                    "Slate",
                    "SlateCore",
                    "EditorStyle",
                    "EditorWidgets",
                    "DesktopWidgets",
                    "PropertyEditor",
                    "SharedSettingsWidgets",
                    "SourceControl",
                    "UnrealEd",
					"InputCore"
                }
            );

            DynamicallyLoadedModuleNames.AddRange(
                new string[]
                {
                    "Settings"
                }
            );

            PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "Settings"
                }
            );
        }
    }
}