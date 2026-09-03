#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/ALSAEQADamageReceiver.h"
#include "Creatures/ALSAEQAGiantSnake.generated.h"

class UALSAEQAHealthComponent;

UCLASS()
class ALSAEQA_API AALSAEQAGiantSnake : public ACharacter, public IALSAEQADamageReceiver
{
    GENERATED_BODY()

public:
    AALSAEQAGiantSnake();

    virtual float ReceiveALSAEQADamage_Implementation(const FALSAEQADamageInfo& DamageInfo) override;

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Snake")
    void StartAmbush();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Snake")
    void CoilAroundObstacle();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Snake")
    void Retreat();

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Snake")
    bool IsDefeated() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Snake")
    TObjectPtr<UALSAEQAHealthComponent> HealthComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Snake")
    float AttackRange = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Snake")
    float BiteDamage = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Snake")
    float ChargeDamage = 55.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Snake")
    float VenomDuration = 4.0f;
};
