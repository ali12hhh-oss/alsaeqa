#include "Interaction/ALSAEQAWorkerPrisonerActor.h"

#include "Player/ALSAEQACharacter.h"
#include "Progression/ALSAEQAProgressionComponent.h"
#include "Progression/ALSAEQAStageObjectiveComponent.h"
#include "Cinematic/ALSAEQACinematicDirector.h"
#include "Save/ALSAEQASaveManager.h"
#include "Engine/GameInstance.h"

AALSAEQAWorkerPrisonerActor::AALSAEQAWorkerPrisonerActor()
{
    PrimaryActorTick.bCanEverTick = false;
    InteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescuePrompt", "إنقاذ العامل");
}

void AALSAEQAWorkerPrisonerActor::BeginPlay()
{
    Super::BeginPlay();
    bRescued = false;

    if (!bCountsAsStageOneWorker || WorkerId.IsNone() || !GetGameInstance())
    {
        return;
    }

    if (UALSAEQASaveManager* SaveManager = GetGameInstance()->GetSubsystem<UALSAEQASaveManager>())
    {
        // A previously rescued worker remains rescued after map reload.
        bRescued = SaveManager->HasStageOneWorkerRescued(WorkerId);
        if (bRescued)
        {
            InteractionPrompt = RescuedInteractionPrompt;
        }
    }
}

void AALSAEQAWorkerPrisonerActor::Interact_Implementation(AActor* Interactor)
{
    Rescue(Interactor);
}

bool AALSAEQAWorkerPrisonerActor::Rescue(AActor* Rescuer)
{
    if (bRescued || !bCountsAsStageOneWorker || WorkerId.IsNone() || !Rescuer)
    {
        return false;
    }

    AALSAEQACharacter* Hero = Cast<AALSAEQACharacter>(Rescuer);
    if (!Hero)
    {
        return false;
    }

    UALSAEQAProgressionComponent* Progression = Hero->GetProgressionComponent();
    UALSAEQAStageObjectiveComponent* Objectives = Hero->GetStageObjectiveComponent();
    if (!Progression || !Objectives || Progression->GetCurrentStage() != 1)
    {
        return false;
    }

    UALSAEQASaveManager* SaveManager = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UALSAEQASaveManager>()
        : nullptr;

    if (SaveManager && SaveManager->HasStageOneWorkerRescued(WorkerId))
    {
        bRescued = true;
        InteractionPrompt = RescuedInteractionPrompt;
        return false;
    }

    // Register the stable identity first. If saving fails, the gameplay
    // objective is not advanced, preventing a false 5/5 state.
    if (SaveManager && !SaveManager->RecordStageOneWorkerRescued(WorkerId))
    {
        return false;
    }

    const bool bProgressRegistered = Objectives->RegisterProgress(TEXT("RescueWorkers"), 1);
    if (!bProgressRegistered)
    {
        return false;
    }

    bRescued = true;
    InteractionPrompt = RescuedInteractionPrompt;

    PlayRescuePresentation(RescueMethod, RescueSequenceTag);

    if (UALSAEQACinematicDirector* Cinematic = Hero->GetCinematicDirector())
    {
        Cinematic->StartStoryBeat(EALSAEQACinematicEvent::Rescue);
    }

    OnWorkerRescued.Broadcast(this);
    return true;
}
