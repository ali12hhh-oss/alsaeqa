#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSAEQAInjuryComponent.generated.h"

UENUM(BlueprintType)
enum class EALSAEQAInjuryBodyPart : uint8
{
    Head,
    Torso,
    LeftArm,
    RightArm,
    LeftLeg,
    RightLeg
};

UENUM(BlueprintType)
enum class EALSAEQAInjuryOrgan : uint8
{
    Heart,
    Lungs,
    Liver,
    None
};

UENUM(BlueprintType)
enum class EALSAEQAInjuryState : uint8
{
    Healthy,
    Injured,
    KnockedOut,
    Critical,
    Dead
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FALSAEQAInjuryStateChanged, EALSAEQAInjuryState, State, float, Severity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FALSAEQAOrganInjured, EALSAEQAInjuryOrgan, Organ);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FALSAEQALimbChanged, EALSAEQAInjuryBodyPart, BodyPart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FALSAEQAInjuryFatalState);

UCLASS(ClassGroup=(ALSAEQA), meta=(BlueprintSpawnableComponent))
class ALSAEQA_API UALSAEQAInjuryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UALSAEQAInjuryComponent();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void ApplyBodyPartInjury(EALSAEQAInjuryBodyPart BodyPart, float Severity);
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void ApplyOrganInjury(EALSAEQAInjuryOrgan Organ, float Severity);
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void SetLimbSevered(EALSAEQAInjuryBodyPart BodyPart, bool bSevered = true);
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void SetKnockedOut(bool bValue = true);
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void RecoverFromKnockout();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void ClearTemporaryInjuries();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void MarkDead();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Injury") void ResetAfterDeath();

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Injury") EALSAEQAInjuryState GetState() const { return State; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Injury") float GetBodyPartSeverity(EALSAEQAInjuryBodyPart BodyPart) const;
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Injury") float GetOrganSeverity(EALSAEQAInjuryOrgan Organ) const;
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Injury") bool IsLimbSevered(EALSAEQAInjuryBodyPart BodyPart) const;
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Injury") bool IsKnockedOut() const { return State == EALSAEQAInjuryState::KnockedOut; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Injury") bool IsCritical() const { return State == EALSAEQAInjuryState::Critical; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Injury") bool IsDead() const { return State == EALSAEQAInjuryState::Dead; }

    UFUNCTION(BlueprintImplementableEvent, Category="ALSAEQA|Injury") void PlayInjuryPresentation(EALSAEQAInjuryBodyPart BodyPart, float Severity);
    UFUNCTION(BlueprintImplementableEvent, Category="ALSAEQA|Injury") void PlayDismembermentPresentation(EALSAEQAInjuryBodyPart BodyPart);
    UFUNCTION(BlueprintImplementableEvent, Category="ALSAEQA|Injury") void PlayKnockoutPresentation(bool bStarted);
    UFUNCTION(BlueprintImplementableEvent, Category="ALSAEQA|Injury") void PlayOrganInjuryPresentation(EALSAEQAInjuryOrgan Organ, float Severity);
    UFUNCTION(BlueprintImplementableEvent, Category="ALSAEQA|Injury") void PlayDeathPresentation();

    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Injury") FALSAEQAInjuryStateChanged OnInjuryStateChanged;
    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Injury") FALSAEQAOrganInjured OnOrganInjured;
    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Injury") FALSAEQALimbChanged OnLimbChanged;
    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Injury") FALSAEQAInjuryFatalState OnFatalState;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Injury") float KnockoutThreshold = 0.90f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Injury") float CriticalThreshold = 0.75f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|Injury") TMap<EALSAEQAInjuryBodyPart, float> BodyPartSeverity;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|Injury") TMap<EALSAEQAInjuryOrgan, float> OrganSeverity;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|Injury") TSet<EALSAEQAInjuryBodyPart> SeveredLimbs;
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|Injury") EALSAEQAInjuryState State = EALSAEQAInjuryState::Healthy;

private:
    void RecalculateState();
};
