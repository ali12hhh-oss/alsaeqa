#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/ALSAEQAEnemyCharacter.generated.h"

class UALSAEQAHealthComponent;

UENUM(BlueprintType)
enum class EALSAEQAEnemyState : uint8
{
    Idle,
    Patrol,
    Alert,
    Chase,
    Attack,
    Stunned,
    Dead
};

UCLASS()
class ALSAEQA_API AALSAEQAEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AALSAEQAEnemyCharacter();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|AI")
    void SetEnemyState(EALSAEQAEnemyState NewState);

    UFUNCTION(BlueprintPure, Category="ALSAEQA|AI")
    EALSAEQAEnemyState GetEnemyState() const { return EnemyState; }

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|AI")
    void SetTargetActor(AActor* NewTarget);

    UFUNCTION(BlueprintPure, Category="ALSAEQA|AI")
    AActor* GetTargetActor() const { return TargetActor.Get(); }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Health")
    UALSAEQAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Components")
    TObjectPtr<UALSAEQAHealthComponent> HealthComponent;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|AI")
    EALSAEQAEnemyState EnemyState = EALSAEQAEnemyState::Idle;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|AI")
    TWeakObjectPtr<AActor> TargetActor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ALSAEQA|AI")
    float ChaseSpeed = 430.0f;
};
