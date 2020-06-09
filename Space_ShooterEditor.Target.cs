using UnrealBuildTool;
using System.Collections.Generic;

public class Space_ShooterEditorTarget : TargetRules
{
	public Space_ShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Space_Shooter" } );
	}
}
