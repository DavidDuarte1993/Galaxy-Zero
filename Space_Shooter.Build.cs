using UnrealBuildTool;

public class Space_Shooter : ModuleRules
{
	public Space_Shooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
