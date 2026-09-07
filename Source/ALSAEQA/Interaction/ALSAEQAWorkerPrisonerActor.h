#pragma once

#include "CoreMinimal.h"
#include "Interaction/ALSAEQAInteractable.h"
#include "ALSAEQAWorkerPrisonerActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FALSAEQAWorkerRescuedSignature, AALSAEQAWorkerPrisonerActor*, Worker);

/**
 * Stage-1 worker/prisoner rescue actor.
 * Place one instance for every worker that must be freed in the mine.
 * Rescue is deliberately one-shot so repeated interaction cannot inflate
 * the RescueWorkers objective.
 */
UCLASS(Blueprintable)
class ALSAEQA_API AALSAEQAWorkerPrisonerActor : public AALSAEQAInteractable
{
    GENERATED_BODY()

public:
    AALSAEQAWorkerPrisonerActor();

    virtual void Interact_Implementation(AActor* Interactor) override;

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Rescue")
    bool Rescue(AActor* Rescuer);

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Rescue")
    bool IsRescued() const { return bRescued; }

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="ALSAEQA|Rescue")
    bool bCountsAsStageOneWorker = true;

    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Rescue")
    FALSAEQAWorkerRescuedSignature OnWorkerRescued;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Rescue")
    FText RescuedInteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescuedPrompt", "تم إنقاذه");

private:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|Rescue", meta=(AllowPrivateAccess="true"))
    bool bRescued = false;
};
