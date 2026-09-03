#include "Companions/ALSAEQAMountActor.h"

#include "Companions/ALSAEQAMountComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AALSAEQAMountActor::AALSAEQAMountActor()
{
    PrimaryActorTick.bCanEverTick = false;
    MountComponent = CreateDefaultSubobject<UALSAEQAMountComponent>(TEXT("MountComponent"));

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = 650.0f;
        GetCharacterMovement()->JumpZVelocity = 650.0f;
    }
}

bool AALSAEQAMountActor::Tame(const FALSAEQAMountProfile& Profile)
{
    return MountComponent && MountComponent->TameMount(Profile);
}

bool AALSAEQAMountActor::MountRider(AActor* NewRider)
{
    if (!NewRider || Rider.IsValid() || !MountComponent || !MountComponent->Mount())
    {
        return false;
    }

    Rider = NewRider;
    NewRider->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    NewRider->SetActorRelativeLocation(RiderOffset);
    return true;
}

bool AALSAEQAMountActor::DismountRider()
{
    if (!Rider.IsValid() || !MountComponent || !MountComponent->Dismount())
    {
        return false;
    }

    AActor* CurrentRider = Rider.Get();
    Rider.Reset();
    CurrentRider->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    return true;
}
