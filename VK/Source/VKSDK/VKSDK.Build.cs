using UnrealBuildTool;
using System.IO;
using System;

public class VKSDK : ModuleRules
{
	public VKSDK(TargetInfo Target)
	{
		Type = ModuleType.External;

		string sdkPath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));
		PublicIncludePaths.Add(Path.Combine(sdkPath, "include"));

		if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicFrameworks.AddRange(new string[]
            {
                "Foundation",
                "CoreFoundation",
                "CoreGraphics",
                "MobileCoreServices",
                "SystemConfiguration",
                "SafariServices",
                "CFNetwork",
                "Security",
                "Foundation",
            });

            string libPath = Path.Combine(sdkPath, "lib");
            string libFilePath = Path.Combine(libPath, "libVKSdk.a");

            PublicAdditionalLibraries.Add("icucore");

            PublicAdditionalLibraries.Add(libFilePath);
            PublicAdditionalShadowFiles.Add(libFilePath);
        }
    }
}
