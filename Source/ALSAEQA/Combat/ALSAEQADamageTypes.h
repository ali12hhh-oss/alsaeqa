#pragma once

#include "CoreMinimal.h"
#include "ALSAEQADamageTypes.generated.h"

UENUM(BlueprintType)
enum class EALSAEQADamageType : uint8
{
    Physical UMETA(DisplayName="Physical"),
    Thunder UMETA(DisplayName="Thunder"),
    Fire UMETA(DisplayName="Fire"),
    Ice UMETA(DisplayName="Ice"),
    Poison UMETA(DisplayName="Poison")
};

USTRUCT(BlueprintType)
struct ALSAEQA_API FALSAEQADamageInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Amount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EALSAEQADamageType Type = EALSAEQADamageType::Physical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<AActor> Instigator = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector HitLocation = FVector::ZeroVector;
};
