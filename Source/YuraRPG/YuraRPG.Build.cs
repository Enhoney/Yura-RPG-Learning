// Copyright Muci Yun.

using UnrealBuildTool;

public class YuraRPG : ModuleRules
{
	public YuraRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput" 
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"YuraRPG",
			"YuraRPG/Public/Character",
			"YuraRPG/Public/Player",
			"YuraRPG/Public/Game",
            "YuraRPG/Public/Interaction",
		});


        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
