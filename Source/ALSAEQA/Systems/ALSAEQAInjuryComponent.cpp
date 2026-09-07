#include "Systems/ALSAEQAInjuryComponent.h"

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

    // Certain vital-organ injuries are fatal. The visual/gameplay presentation remains
    // Blueprint-driven so humanoids, creatures and bosses can each respond appropriately.
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
        SeveredLimbs.Add(BodyPart);
        BodyPartSeverity.FindOrAdd(BodyPart) = 1.0f;
        PlayDismembermentPresentation(BodyPart);
    }
    else
    {
        SeveredLimbs.Remove(BodyPart);
    }

    OnLimbChanged.Broadcast(BodyPart);
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
            PlayKnockoutPresentation(true);
            OnInjuryStateChanged.Broadcast(State, 1.0f);
        }
    }
    else
    {
        RecoverFromKnockout();
    }
}

void UALSAEQAInjuryComponent::RecoverFromKnockout()
{
    if (State != EALSAEQAInjuryState::KnockedOut) return;
    PlayKnockoutPresentation(false);
    RecalculateState();
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
    if (State == EALSAEQAInjuryState::Dead) return;

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
        OnInjuryStateChanged.Broadcast(State, Highest);
        if (State == EALSAEQAInjuryState::KnockedOut) PlayKnockoutPresentation(true);
    }
}
