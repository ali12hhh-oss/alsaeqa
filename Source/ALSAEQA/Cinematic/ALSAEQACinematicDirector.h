#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cinematic/ALSAEQACinematicTypes.h"
#include "ALSAEQACinematicDirector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FALSAEQACinematicEvent, EALSAEQACinematicEvent, Event);

UCLASS(ClassGroup=(ALSAEQA), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class ALSAEQA_API UALSAEQACinematicDirector : public UActorComponent
{
    GENERATED_BODY()

public:
    UALSAEQACinematicDirector();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Cinematics")
    bool PlayActionMoment(const FALSAEQACinematicRequest& Request);

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Cinematics")
    void StopActionMoment();

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Cinematics")
    bool IsActionMomentActive() const { return bActionMomentActive; }

    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Cinematics")
    FALSAEQACinematicEvent OnCinematicEvent;

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle RestoreTimeHandle;
    float PreviousTimeDilation = 1.0f;
    bool bActionMomentActive = false;

    void RestoreNormalTime();
};
