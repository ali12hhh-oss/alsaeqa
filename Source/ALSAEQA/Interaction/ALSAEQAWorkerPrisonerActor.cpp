#include "Interaction/ALSAEQAWorkerPrisonerActor.h"

#include "Player/ALSAEQACharacter.h"
#include "Progression/ALSAEQAProgressionComponent.h"
#include "Progression/ALSAEQAStageObjectiveComponent.h"
#include "Cinematic/ALSAEQACinematicDirector.h"

AALSAEQAWorkerPrisonerActor::AALSAEQAWorkerPrisonerActor()
{
    PrimaryActorTick.bCanEverTick = false;
    InteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescuePrompt", "إنقاذ العامل");
}

void AALSAEQAWorkerPrisonerActor::BeginPlay()
{
    Super::BeginPlay();
    bRescued = false;
}

void AALSAEQAWorkerPrisonerActor::Interact_Implementation(AActor* Interactor)
{
    Rescue(Interactor);
}

bool AALSAEQAWorkerPrisonerActor::Rescue(AActor* Rescuer)
{
    if (bRescued || !bCountsAsStageOneWorker || !Rescuer)
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

    bRescued = true;
    InteractionPrompt = RescuedInteractionPrompt;

    const bool bProgressRegistered = Objectives->RegisterProgress(TEXT("RescueWorkers"), 1);
    if (!bProgressRegistered)
    {
        bRescued = false;
        InteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescuePrompt", "إنقاذ العامل");
        return false;
    }

    PlayRescuePresentation(RescueMethod, RescueSequenceTag);

    if (UALSAEQACinematicDirector* Cinematic = Hero->GetCinematicDirector())
    {
        Cinematic->StartStoryBeat(EALSAEQACinematicEvent::Rescue);
    }

    OnWorkerRescued.Broadcast(this);
    return true;
}
