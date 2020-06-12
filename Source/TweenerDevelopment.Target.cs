// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TweenerDevelopmentTarget : TargetRules
{
	public TweenerDevelopmentTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "TweenerDevelopment" } );
	}
}
