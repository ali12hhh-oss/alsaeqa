#include "Interaction/ALSAEQAWorkerPrisonerActor.h"

#include "Player/ALSAEQACharacter.h"
#include "Progression/ALSAEQAProgressionComponent.h"
#include "Progression/ALSAEQAStageObjectiveComponent.h"
#include "Cinematic/ALSAEQACinematicDirector.h"
#include "Save/ALSAEQASaveManager.h"
#include "AI/ALSAEQAEnemyCharacter.h"
#include "EngineUtils.h"
#include "Engine/GameInstance.h"

AALSAEQAWorkerPrisonerActor::AALSAEQAWorkerPrisonerActor()
{
    PrimaryActorTick.bCanEverTick = true;
    InteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescuePrompt", "إنقاذ العامل");
}

void AALSAEQAWorkerPrisonerActor::BeginPlay()
{
    Super::BeginPlay();
    bRescued = false;
    bRescueInProgress = false;

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

void AALSAEQAWorkerPrisonerActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bRescueInProgress && IsRescueThreatening())
    {
        CancelRescue();
    }
}

void AALSAEQAWorkerPrisonerActor::Interact_Implementation(AActor* Interactor)
{
    Rescue(Interactor);
}

bool AALSAEQAWorkerPrisonerActor::IsRescueThreatening() const
{
    if (!bRescueInProgress)
    {
        return false;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return true;
    }

    const FVector WorkerLocation = GetActorLocation();
    for (TActorIterator<AALSAEQAEnemyCharacter> It(World); It; ++It)
    {
        const AALSAEQAEnemyCharacter* Enemy = *It;
        if (!IsValid(Enemy) || Enemy->GetEnemyState() == EALSAEQAEnemyState::Dead || Enemy->GetEnemyState() == EALSAEQAEnemyState::Stunned)
        {
            continue;
        }

        const bool bNearWorker = FVector::DistSquared2D(Enemy->GetActorLocation(), WorkerLocation)
            <= FMath::Square(RescueThreatRadius);
        const bool bTargetingRescuer = RescueInstigator.IsValid() && Enemy->GetTargetActor() == RescueInstigator.Get();

        if (bNearWorker || bTargetingRescuer)
        {
            return true;
        }
    }

    return false;
}

bool AALSAEQAWorkerPrisonerActor::Rescue(AActor* Rescuer)
{
    if (bRescued || bRescueInProgress || !bCountsAsStageOneWorker || WorkerId.IsNone() || !Rescuer)
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

    // A rescue cannot begin while an active hostile is already on top of the
    // worker or actively targeting the rescuer. The player must create space
    // first by fighting, stunning, repositioning, or otherwise controlling the threat.
    RescueInstigator = Hero;
    if (IsRescueThreatening())
    {
        RescueInstigator.Reset();
        return false;
    }

    bRescueInProgress = true;
    InteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescueInProgressPrompt", "جارٍ الإنقاذ...");

    PlayRescuePresentation(RescueMethod, RescueSequenceTag);

    GetWorldTimerManager().SetTimer(
        RescueTimerHandle,
        this,
        &AALSAEQAWorkerPrisonerActor::FinishRescue,
        FMath::Max(0.25f, RescueDuration),
        false);

    return true;
}

void AALSAEQAWorkerPrisonerActor::FinishRescue()
{
    if (!bRescueInProgress || bRescued)
    {
        return;
    }

    if (IsRescueThreatening())
    {
        CancelRescue();
        return;
    }

    AALSAEQACharacter* Hero = Cast<AALSAEQACharacter>(RescueInstigator.Get());
    if (!Hero)
    {
        CancelRescue();
        return;
    }

    UALSAEQAProgressionComponent* Progression = Hero->GetProgressionComponent();
    UALSAEQAStageObjectiveComponent* Objectives = Hero->GetStageObjectiveComponent();
    if (!Progression || !Objectives || Progression->GetCurrentStage() != 1)
    {
        CancelRescue();
        return;
    }

    UALSAEQASaveManager* SaveManager = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UALSAEQASaveManager>()
        : nullptr;

    if (SaveManager && SaveManager->HasStageOneWorkerRescued(WorkerId))
    {
        bRescueInProgress = false;
        RescueInstigator.Reset();
        InteractionPrompt = RescuedInteractionPrompt;
        return;
    }

    // Commit the stable worker identity only after the physical rescue window
    // completes successfully, so an interrupted interaction cannot count.
    if (SaveManager && !SaveManager->RecordStageOneWorkerRescued(WorkerId))
    {
        CancelRescue();
        return;
    }

    if (!Objectives->RegisterProgress(TEXT("RescueWorkers"), 1))
    {
        // Do not present a successful rescue if the stage system rejected it.
        CancelRescue();
        return;
    }

    bRescueInProgress = false;
    bRescued = true;
    RescueInstigator.Reset();
    InteractionPrompt = RescuedInteractionPrompt;

    if (UALSAEQACinematicDirector* Cinematic = Hero->GetCinematicDirector())
    {
        Cinematic->StartStoryBeat(EALSAEQACinematicEvent::Rescue);
    }

    OnWorkerRescued.Broadcast(this);
}

void AALSAEQAWorkerPrisonerActor::CancelRescue()
{
    if (!bRescueInProgress)
    {
        return;
    }

    GetWorldTimerManager().ClearTimer(RescueTimerHandle);
    bRescueInProgress = false;
    RescueInstigator.Reset();
    InteractionPrompt = NSLOCTEXT("ALSAEQA", "WorkerRescuePrompt", "إنقاذ العامل");
    PlayRescueInterruptedPresentation();
}
