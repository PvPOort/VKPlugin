// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class VK : ModuleRules
	{
		public VK(TargetInfo Target)
		{
            Definitions.Add("WITH_VKONTAKTE=1");

			PublicIncludePaths.AddRange(
				new string[] {
					"Runtime/VK/Public",
				});

			PrivateIncludePaths.AddRange(
				new string[] {
					"Runtime/VK/Private",
				});

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
				});

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
					"CoreUObject",
                    "Engine",
				});

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
				});

            if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                PublicFrameworks.AddRange(new string[]
                {
                    "Foundation",
                    "UIKit",
                    "CoreFoundation",
                });

                AddThirdPartyPrivateStaticDependencies(Target, "VKSDK");
            }
        }
	}
}