#pragma once

#include "CoreMinimal.h"
#include "Interaction/ALSAEQAInteractable.h"
#include "ALSAEQAWorkerPrisonerActor.generated.h"

class AALSAEQAWorkerPrisonerActor;

UENUM(BlueprintType)
enum class EALSAEQAWorkerRescueMethod : uint8
{
    BreakChain UMETA(DisplayName="Break Chain"),
    OpenCage UMETA(DisplayName="Open Cage"),
    ReleaseLift UMETA(DisplayName="Release Lift"),
    CutBinding UMETA(DisplayName="Cut Binding"),
    EscortOut UMETA(DisplayName="Escort Out")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FALSAEQAWorkerRescuedSignature, AALSAEQAWorkerPrisonerActor*, Worker);

/**
 * Stage-1 worker/prisoner rescue actor.
 * Place one instance for every worker that must be freed in the mine.
 * Rescue is deliberately one-shot so repeated interaction cannot inflate
 * the RescueWorkers objective.
 *
 * Each worker can use a different authored rescue method. The method and
 * RescueSequenceTag are exposed to Blueprint so the real cage/chain/lift
 * assets and animation can provide distinct presentations without changing
 * the progression rules.
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

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Rescue")
    EALSAEQAWorkerRescueMethod GetRescueMethod() const { return RescueMethod; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Rescue")
    FName GetRescueSequenceTag() const { return RescueSequenceTag; }

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="ALSAEQA|Rescue")
    bool bCountsAsStageOneWorker = true;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="ALSAEQA|Rescue")
    EALSAEQAWorkerRescueMethod RescueMethod = EALSAEQAWorkerRescueMethod::BreakChain;

    /** Optional authored identifier, e.g. Worker_Cage_01 or Worker_Lift_02. */
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="ALSAEQA|Rescue")
    FName RescueSequenceTag = NAME_None;

    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Rescue")
    FALSAEQAWorkerRescuedSignature OnWorkerRescued;

    /** Lets the real Blueprint presentation animate the specific rescue method. */
    UFUNCTION(BlueprintImplementableEvent, Category="ALSAEQA|Rescue")
    void PlayRescuePresentation(EALSAEQAWorkerRescueMethod Method, FName SequenceTag);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Rescue")
    FText RescuedInteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescuedPrompt", "تم إنقاذه");

private:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="ALSAEQA|Rescue", meta=(AllowPrivateAccess="true"))
    bool bRescued = false;
};
