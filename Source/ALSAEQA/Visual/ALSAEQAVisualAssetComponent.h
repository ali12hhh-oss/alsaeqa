#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSAEQAVisualAssetComponent.generated.h"

class USkeletalMesh;
class UAnimInstance;

/**
 * Visual-only bridge between ALSAEQA gameplay actors and licensed art assets.
 * Gameplay code remains independent from a specific marketplace character.
 */
UCLASS(ClassGroup=(ALSAEQA), meta=(BlueprintSpawnableComponent))
class ALSAEQA_API UALSAEQAVisualAssetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UALSAEQAVisualAssetComponent();

    /** Applies the configured real skeletal mesh/animation class to an owning Character. */
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Visual") bool ApplyVisualAsset();

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Visual") bool HasConfiguredVisualAsset() const;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ALSAEQA|Visual")
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ALSAEQA|Visual")
    TSoftClassPtr<UAnimInstance> AnimationClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ALSAEQA|Visual")
    bool bApplyOnBeginPlay = true;
};
