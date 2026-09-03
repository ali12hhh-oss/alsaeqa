using UnrealBuildTool;

public class ALSAEQA : ModuleRules
{
    public ALSAEQA(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "EnhancedInput"
        });
    }
}
