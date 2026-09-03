#include "Companions/ALSAEQAMountComponent.h"

UALSAEQAMountComponent::UALSAEQAMountComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAMountComponent::SetState(EALSAEQAMountState NewState)
{
    if (MountState == NewState)
    {
        return;
    }

    MountState = NewState;
    OnStateChanged.Broadcast(NewState);
}

bool UALSAEQAMountComponent::TameMount(const FALSAEQAMountProfile& Profile)
{
    if (Profile.MountId.IsNone() || MountState == EALSAEQAMountState::Mounted)
    {
        return false;
    }

    MountProfile = Profile;
    SetState(EALSAEQAMountState::Tamed);
    return true;
}

bool UALSAEQAMountComponent::Mount()
{
    if (!HasTamedMount())
    {
        return false;
    }

    SetState(EALSAEQAMountState::Mounted);
    OnMountChanged.Broadcast(MountProfile.MountId, true);
    return true;
}

bool UALSAEQAMountComponent::Dismount()
{
    if (!IsMounted())
    {
        return false;
    }

    SetState(EALSAEQAMountState::Tamed);
    OnMountChanged.Broadcast(MountProfile.MountId, false);
    return true;
}

bool UALSAEQAMountComponent::ReleaseMount()
{
    if (!HasTamedMount())
    {
        return false;
    }

    const FName ReleasedMountId = MountProfile.MountId;
    MountProfile = FALSAEQAMountProfile();
    SetState(EALSAEQAMountState::Released);
    OnMountChanged.Broadcast(ReleasedMountId, false);
    return true;
}
