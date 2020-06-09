using UnrealBuildTool;
using System.Collections.Generic;

public class Space_ShooterTarget : TargetRules
{
	public Space_ShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Space_Shooter" } );
	}
}
