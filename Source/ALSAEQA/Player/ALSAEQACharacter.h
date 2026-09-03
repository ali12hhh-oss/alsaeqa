#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALSAEQACharacter.generated.h"

class UALSAEQAAbilityComponent;
class UALSAEQAHealthComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ALSAEQA_API AALSAEQACharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AALSAEQACharacter();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Combat")
    void PerformLightAttack();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Combat")
    void PerformHeavyAttack();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Movement")
    void StopSprint();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Abilities")
    bool ActivateAbility(EALSAEQAAbility Ability);

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Abilities")
    UALSAEQAAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Health")
    UALSAEQAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
    virtual void BeginPlay() override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components")
    TObjectPtr<UALSAEQAAbilityComponent> AbilityComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components")
    TObjectPtr<UALSAEQAHealthComponent> HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Movement")
    float WalkSpeed = 360.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|Movement")
    float SprintSpeed = 620.0f;
};
