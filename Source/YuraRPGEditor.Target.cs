// Copyright Muci Yun.

using UnrealBuildTool;
using System.Collections.Generic;

public class YuraRPGEditorTarget : TargetRules
{
	public YuraRPGEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("YuraRPG");
	}
}
