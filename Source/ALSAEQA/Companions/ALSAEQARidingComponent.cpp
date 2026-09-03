#include "Companions/ALSAEQARidingComponent.h"

#include "Companions/ALSAEQAMountActor.h"

UALSAEQARidingComponent::UALSAEQARidingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UALSAEQARidingComponent::TryMount(AALSAEQAMountActor* Mount)
{
    if (!Mount || IsRiding())
    {
        return false;
    }

    if (!Mount->MountRider(GetOwner()))
    {
        return false;
    }

    CurrentMount = Mount;
    OnRidingStateChanged.Broadcast(true);
    return true;
}

bool UALSAEQARidingComponent::Dismount()
{
    AALSAEQAMountActor* Mount = CurrentMount.Get();
    if (!Mount)
    {
        CurrentMount.Reset();
        return false;
    }

    if (!Mount->DismountRider())
    {
        return false;
    }

    CurrentMount.Reset();
    OnRidingStateChanged.Broadcast(false);
    return true;
}
