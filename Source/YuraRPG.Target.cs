// Copyright Muci Yun.

using UnrealBuildTool;
using System.Collections.Generic;

public class YuraRPGTarget : TargetRules
{
	public YuraRPGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("YuraRPG");
	}
}
