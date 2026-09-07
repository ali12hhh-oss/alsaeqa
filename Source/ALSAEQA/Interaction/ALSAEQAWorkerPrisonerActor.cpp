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

    RescueInstigator = Hero;
    bRescueInProgress = true;

    // The threat test is now performed while the rescue is marked active,
    // so a guard that is already close to the worker blocks the interaction.
    if (IsRescueThreatening())
    {
        bRescueInProgress = false;
        RescueInstigator.Reset();
        return false;
    }

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

    if (SaveManager && !SaveManager->RecordStageOneWorkerRescued(WorkerId))
    {
        CancelRescue();
        return;
    }

    if (!Objectives->RegisterProgress(TEXT("RescueWorkers"), 1))
    {
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
