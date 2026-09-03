#include "Progression/ALSAEQAProgressionComponent.h"

UALSAEQAProgressionComponent::UALSAEQAProgressionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UALSAEQAProgressionComponent::UnlockPower(EALSAEQAPower Power, FName DiscoveryId)
{
    if (DiscoveryId.IsNone() || UnlockedPowers.Contains(Power))
    {
        return false;
    }

    UnlockedPowers.Add(Power);
    OnProgressionChanged.ExecuteIfBound();
    return true;
}

bool UALSAEQAProgressionComponent::AcquireWeapon(EALSAEQAWeapon Weapon, FName DiscoveryId)
{
    if (DiscoveryId.IsNone() || AcquiredWeapons.Contains(Weapon))
    {
        return false;
    }

    AcquiredWeapons.Add(Weapon);
    OnProgressionChanged.ExecuteIfBound();
    return true;
}

bool UALSAEQAProgressionComponent::AdvanceStage(int32 NewStage)
{
    if (NewStage <= CurrentStage)
    {
        return false;
    }

    CurrentStage = NewStage;
    OnProgressionChanged.ExecuteIfBound();
    return true;
}

bool UALSAEQAProgressionComponent::HasPower(EALSAEQAPower Power) const
{
    return UnlockedPowers.Contains(Power);
}

bool UALSAEQAProgressionComponent::HasWeapon(EALSAEQAWeapon Weapon) const
{
    return AcquiredWeapons.Contains(Weapon);
}

bool UALSAEQAProgressionComponent::IsPowerAvailable(EALSAEQAPower Power) const
{
    return HasPower(Power);
}

bool UALSAEQAProgressionComponent::IsWeaponAvailable(EALSAEQAWeapon Weapon) const
{
    return HasWeapon(Weapon);
}

void UALSAEQAProgressionComponent::ResetProgression()
{
    CurrentStage = 0;
    UnlockedPowers.Reset();
    AcquiredWeapons.Reset();
    OnProgressionChanged.ExecuteIfBound();
}
