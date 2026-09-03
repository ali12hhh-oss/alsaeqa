#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Systems/ALSAEQAAbilityComponent.h"
#include "Companions/ALSAEQAMountAbilityComponent.h"
#include "ALSAEQACharacter.generated.h"

class UALSAEQAHealthComponent;
class UALSAEQAThunderChargeComponent;
class UALSAEQALegacyComponent;
class UALSAEQARidingComponent;
class USpringArmComponent;
class UCameraComponent;
class AALSAEQAMountActor;

UCLASS()
class ALSAEQA_API AALSAEQACharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AALSAEQACharacter();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Combat") void PerformLightAttack();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Combat") void PerformHeavyAttack();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Thunder") void BeginThunderCharge();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Thunder") void ReleaseThunderCharge();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Thunder") void CancelThunderCharge();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Movement") void StartSprint();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Movement") void StopSprint();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Riding") bool MountOrDismount();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Riding") bool MountNearestTamedMount();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Riding") bool DismountCurrentMount();
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Riding") bool IsRiding() const;
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Abilities") bool ActivateAbility(EALSAEQAAbility Ability);
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|MountAbility") bool ActivateMountAbility(EALSAEQAMountAbility Ability);
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Riding") UALSAEQARidingComponent* GetRidingComponent() const { return RidingComponent; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Abilities") UALSAEQAAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Health") UALSAEQAHealthComponent* GetHealthComponent() const { return HealthComponent; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Thunder") UALSAEQAThunderChargeComponent* GetThunderChargeComponent() const { return ThunderChargeComponent; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Legacy") UALSAEQALegacyComponent* GetLegacyComponent() const { return LegacyComponent; }

protected:
    virtual void BeginPlay() override;
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);
    void ActivateMountLightningDash();
    void ActivateMountThunderRoar();
    void ActivateMountLightningKick();
    void ActivateMountStormCharge();
    void ActivateMountLightningCrossing();
    void ActivateMountStormLeap();
    void ActivateMountLightningShield();
    void ActivateMountStormSummon();
    void ActivateMountThunderSense();
    void ActivateMountStormMode();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components") TObjectPtr<USpringArmComponent> CameraBoom;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components") TObjectPtr<UCameraComponent> FollowCamera;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components") TObjectPtr<UALSAEQAAbilityComponent> AbilityComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components") TObjectPtr<UALSAEQAHealthComponent> HealthComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components") TObjectPtr<UALSAEQAThunderChargeComponent> ThunderChargeComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components") TObjectPtr<UALSAEQALegacyComponent> LegacyComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components") TObjectPtr<UALSAEQARidingComponent> RidingComponent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Movement") float WalkSpeed = 360.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Movement") float SprintSpeed = 620.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Riding") float MountSearchRadius = 450.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Thunder") float ThunderReleaseDamage = 30.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Thunder") float ThunderAttackRange = 650.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Thunder") float ThunderAttackRadius = 90.0f;

private:
    int32 ApplyThunderReleaseToTargets(float Damage);
};
