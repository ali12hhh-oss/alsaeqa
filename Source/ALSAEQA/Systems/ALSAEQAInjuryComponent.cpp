#include "Systems/ALSAEQAInjuryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

UALSAEQAInjuryComponent::UALSAEQAInjuryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAInjuryComponent::ApplyBodyPartInjury(EALSAEQAInjuryBodyPart BodyPart, float Severity)
{
    if (State == EALSAEQAInjuryState::Dead || Severity <= 0.0f) return;
    const float NewSeverity = FMath::Clamp(GetBodyPartSeverity(BodyPart) + Severity, 0.0f, 1.0f);
    BodyPartSeverity.FindOrAdd(BodyPart) = NewSeverity;
    PlayInjuryPresentation(BodyPart, NewSeverity);
    RecalculateState();
}

void UALSAEQAInjuryComponent::ApplyOrganInjury(EALSAEQAInjuryOrgan Organ, float Severity)
{
    if (Organ == EALSAEQAInjuryOrgan::None || State == EALSAEQAInjuryState::Dead || Severity <= 0.0f) return;
    const float NewSeverity = FMath::Clamp(GetOrganSeverity(Organ) + Severity, 0.0f, 1.0f);
    OrganSeverity.FindOrAdd(Organ) = NewSeverity;
    PlayOrganInjuryPresentation(Organ, NewSeverity);
    OnOrganInjured.Broadcast(Organ);
    if ((Organ == EALSAEQAInjuryOrgan::Heart || Organ == EALSAEQAInjuryOrgan::Lungs) && NewSeverity >= 1.0f)
    {
        MarkDead();
        return;
    }
    RecalculateState();
}

void UALSAEQAInjuryComponent::SetLimbSevered(EALSAEQAInjuryBodyPart BodyPart, bool bSevered)
{
    const bool bLimb = BodyPart == EALSAEQAInjuryBodyPart::LeftArm || BodyPart == EALSAEQAInjuryBodyPart::RightArm || BodyPart == EALSAEQAInjuryBodyPart::LeftLeg || BodyPart == EALSAEQAInjuryBodyPart::RightLeg;
    if (!bLimb || State == EALSAEQAInjuryState::Dead) return;
    if (bSevered)
    {
        if (!SeveredLimbs.Contains(BodyPart))
        {
            SeveredLimbs.Add(BodyPart);
            BodyPartSeverity.FindOrAdd(BodyPart) = 1.0f;
            ApplySeveredLimbVisual(BodyPart);
            PlayDismembermentPresentation(BodyPart);
            OnLimbChanged.Broadcast(BodyPart);
        }
    }
    else if (SeveredLimbs.Remove(BodyPart) > 0)
    {
        RestoreLimbVisual(BodyPart);
        OnLimbChanged.Broadcast(BodyPart);
    }
    RecalculateState();
}

void UALSAEQAInjuryComponent::SetKnockedOut(bool bValue)
{
    if (State == EALSAEQAInjuryState::Dead) return;
    if (bValue)
    {
        if (State != EALSAEQAInjuryState::KnockedOut)
        {
            State = EALSAEQAInjuryState::KnockedOut;
            ApplyOwnerStatePresentation();
            PlayKnockoutPresentation(true);
            OnInjuryStateChanged.Broadcast(State, 1.0f);
        }
    }
    else RecoverFromKnockout();
}

void UALSAEQAInjuryComponent::RecoverFromKnockout()
{
    if (State != EALSAEQAInjuryState::KnockedOut) return;
    PlayKnockoutPresentation(false);
    RecalculateState();
    ApplyOwnerStatePresentation();
}

void UALSAEQAInjuryComponent::ClearTemporaryInjuries()
{
    if (State == EALSAEQAInjuryState::Dead) return;
    BodyPartSeverity.Reset();
    OrganSeverity.Reset();
    State = SeveredLimbs.Num() > 0 ? EALSAEQAInjuryState::Injured : EALSAEQAInjuryState::Healthy;
    OnInjuryStateChanged.Broadcast(State, 0.0f);
}

void UALSAEQAInjuryComponent::MarkDead()
{
    if (State == EALSAEQAInjuryState::Dead) return;
    State = EALSAEQAInjuryState::Dead;
    ApplyOwnerStatePresentation();
    OnInjuryStateChanged.Broadcast(State, 1.0f);
    PlayDeathPresentation();
    OnFatalState.Broadcast();
}

void UALSAEQAInjuryComponent::ResetAfterDeath()
{
    BodyPartSeverity.Reset();
    OrganSeverity.Reset();
    SeveredLimbs.Reset();
    State = EALSAEQAInjuryState::Healthy;
    if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
    {
        if (UCharacterMovementComponent* Movement = Character->GetCharacterMovement())
        {
            Movement->SetMovementMode(MOVE_Walking);
        }
        if (USkeletalMeshComponent* Mesh = Character->GetMesh())
        {
            Mesh->SetSimulatePhysics(false);
            Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
    }
    OnInjuryStateChanged.Broadcast(State, 0.0f);
}

float UALSAEQAInjuryComponent::GetBodyPartSeverity(EALSAEQAInjuryBodyPart BodyPart) const
{
    if (const float* Value = BodyPartSeverity.Find(BodyPart)) return *Value;
    return 0.0f;
}

float UALSAEQAInjuryComponent::GetOrganSeverity(EALSAEQAInjuryOrgan Organ) const
{
    if (const float* Value = OrganSeverity.Find(Organ)) return *Value;
    return 0.0f;
}

bool UALSAEQAInjuryComponent::IsLimbSevered(EALSAEQAInjuryBodyPart BodyPart) const
{
    return SeveredLimbs.Contains(BodyPart);
}

void UALSAEQAInjuryComponent::RecalculateState()
{
    if (State == EALSAEQAInjuryState::Dead || State == EALSAEQAInjuryState::KnockedOut) return;
    float Highest = 0.0f;
    for (const TPair<EALSAEQAInjuryBodyPart, float>& Pair : BodyPartSeverity) Highest = FMath::Max(Highest, Pair.Value);
    for (const TPair<EALSAEQAInjuryOrgan, float>& Pair : OrganSeverity) Highest = FMath::Max(Highest, Pair.Value);

    EALSAEQAInjuryState NewState = EALSAEQAInjuryState::Healthy;
    if (Highest >= CriticalThreshold) NewState = EALSAEQAInjuryState::Critical;
    else if (Highest > 0.0f) NewState = EALSAEQAInjuryState::Injured;
    if (Highest >= KnockoutThreshold) NewState = EALSAEQAInjuryState::KnockedOut;

    if (NewState != State)
    {
        State = NewState;
        ApplyOwnerStatePresentation();
        OnInjuryStateChanged.Broadcast(State, Highest);
        if (State == EALSAEQAInjuryState::KnockedOut) PlayKnockoutPresentation(true);
    }
}

void UALSAEQAInjuryComponent::ApplyOwnerStatePresentation()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement) return;

    if (State == EALSAEQAInjuryState::KnockedOut || State == EALSAEQAInjuryState::Dead)
    {
        Movement->StopMovementImmediately();
        Movement->DisableMovement();
        if (State == EALSAEQAInjuryState::Dead && bAutoRagdollOnDeath)
        {
            if (USkeletalMeshComponent* Mesh = Character->GetMesh())
            {
                Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
                Mesh->SetSimulatePhysics(true);
                Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
        }
    }
    else if (State == EALSAEQAInjuryState::Critical)
    {
        Movement->MaxWalkSpeed = FMath::Min(Movement->MaxWalkSpeed, 180.0f);
    }
}

void UALSAEQAInjuryComponent::ApplySeveredLimbVisual(EALSAEQAInjuryBodyPart BodyPart)
{
    if (!bAutoHideSeveredLimbBones) return;
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    USkeletalMeshComponent* Mesh = Character ? Character->GetMesh() : nullptr;
    if (!Mesh) return;
    FName Bone = NAME_None;
    switch (BodyPart)
    {
        case EALSAEQAInjuryBodyPart::LeftArm: Bone = LeftArmBone; break;
        case EALSAEQAInjuryBodyPart::RightArm: Bone = RightArmBone; break;
        case EALSAEQAInjuryBodyPart::LeftLeg: Bone = LeftLegBone; break;
        case EALSAEQAInjuryBodyPart::RightLeg: Bone = RightLegBone; break;
        default: break;
    }
    if (!Bone.IsNone() && Mesh->GetBoneIndex(Bone) != INDEX_NONE) Mesh->HideBoneByName(Bone, EPhysBodyOp::PBO_None);
}

void UALSAEQAInjuryComponent::RestoreLimbVisual(EALSAEQAInjuryBodyPart BodyPart)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    USkeletalMeshComponent* Mesh = Character ? Character->GetMesh() : nullptr;
    if (!Mesh) return;
    FName Bone = NAME_None;
    switch (BodyPart)
    {
        case EALSAEQAInjuryBodyPart::LeftArm: Bone = LeftArmBone; break;
        case EALSAEQAInjuryBodyPart::RightArm: Bone = RightArmBone; break;
        case EALSAEQAInjuryBodyPart::LeftLeg: Bone = LeftLegBone; break;
        case EALSAEQAInjuryBodyPart::RightLeg: Bone = RightLegBone; break;
        default: break;
    }
    if (!Bone.IsNone() && Mesh->GetBoneIndex(Bone) != INDEX_NONE) Mesh->UnHideBoneByName(Bone);
}
