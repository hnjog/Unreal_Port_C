using UnrealBuildTool;

public class Unreal_Port_Code : ModuleRules
{
    public Unreal_Port_Code(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule",
            "GameplayTasks"
        });

        PublicIncludePaths.Add(ModuleDirectory);
        //PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
}
