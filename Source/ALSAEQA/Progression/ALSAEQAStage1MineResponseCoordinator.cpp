#include "Progression/ALSAEQAStage1MineResponseCoordinator.h"

#include "Interaction/ALSAEQAWorkerPrisonerActor.h"
#include "AI/ALSAEQAEnemyCharacter.h"
#include "EngineUtils.h"
#include "Engine/GameInstance.h"
#include "Save/ALSAEQASaveManager.h"
#include "Progression/ALSAEQAProgressionComponent.h"
#include "Player/ALSAEQACharacter.h"

UALSAEQAStage1MineResponseCoordinator::UALSAEQAStage1MineResponseCoordinator()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAStage1MineResponseCoordinator::BeginPlay()
{
    Super::BeginPlay();

    if (!GetOwner() || !GetOwner()->GetWorld()) return;

    for (TActorIterator<AALSAEQAWorkerPrisonerActor> It(GetOwner()->GetWorld()); It; ++It)
    {
        RegisterWorker(*It);
    }

    for (TActorIterator<AALSAEQAEnemyCharacter> It(GetOwner()->GetWorld()); It; ++It)
    {
        RegisterMineGuard(*It);
    }

    int32 SavedWorkers = 0;
    if (UGameInstance* GI = GetOwner()->GetGameInstance())
    {
        if (UALSAEQASaveManager* Save = GI->GetSubsystem<UALSAEQASaveManager>())
        {
            SavedWorkers = Save->GetStageOneWorkersRescuedCount();
        }
    }
    WorkersRescued = FMath::Clamp(SavedWorkers, 0, Workers.Num());
    ResponseLevel = FMath::Clamp(WorkersRescued, 0, MaxResponseLevel);
    RefreshGuardAssignments();
}

void UALSAEQAStage1MineResponseCoordinator::RegisterWorker(AALSAEQAWorkerPrisonerActor* Worker)
{
    if (!IsValid(Worker) || Workers.Contains(Worker) || !Worker->bCountsAsStageOneWorker) return;
    Workers.Add(Worker);
    Worker->OnWorkerRescued.AddDynamic(this, &UALSAEQAStage1MineResponseCoordinator::HandleWorkerRescued);
}

void UALSAEQAStage1MineResponseCoordinator::RegisterMineGuard(AALSAEQAEnemyCharacter* Guard)
{
    if (!IsValid(Guard) || Guards.Contains(Guard)) return;
    Guards.Add(Guard);
}

void UALSAEQAStage1MineResponseCoordinator::HandleWorkerRescued(AALSAEQAWorkerPrisonerActor* Worker)
{
    if (!IsValid(Worker)) return;

    WorkersRescued = FMath::Clamp(WorkersRescued + 1, 0, Workers.Num());
    ResponseLevel = FMath::Clamp(WorkersRescued, 0, MaxResponseLevel);

    RefreshGuardAssignments();
    PlayMineResponsePresentation(ResponseLevel, Worker);
}

void UALSAEQAStage1MineResponseCoordinator::RefreshGuardAssignments()
{
    if (!GetOwner() || !GetOwner()->GetWorld()) return;

    AALSAEQACharacter* Hero = nullptr;
    for (TActorIterator<AALSAEQACharacter> It(GetOwner()->GetWorld()); It; ++It)
    {
        if (IsValid(*It)) { Hero = *It; break; }
    }

    TArray<AALSAEQAWorkerPrisonerActor*> EscapingWorkers;
    for (AALSAEQAWorkerPrisonerActor* Worker : Workers)
    {
        if (IsValid(Worker) && Worker->GetRescueState() == EALSAEQAWorkerRescueState::Escaping)
        {
            EscapingWorkers.Add(Worker);
        }
    }

    if (EscapingWorkers.Num() == 0) return;

    int32 Assigned = 0;
    const int32 DesiredAssignments = FMath::Min(Guards.Num(), ResponseLevel * FMath::Max(1, GuardsPerResponseLevel));

    for (AALSAEQAEnemyCharacter* Guard : Guards)
    {
        if (!IsValid(Guard) || Guard->GetEnemyState() == EALSAEQAEnemyState::Dead) continue;
        if (Assigned >= DesiredAssignments) break;

        AALSAEQAWorkerPrisonerActor* NearestWorker = nullptr;
        float BestDistanceSq = TNumericLimits<float>::Max();
        for (AALSAEQAWorkerPrisonerActor* Worker : EscapingWorkers)
        {
            const float DistanceSq = FVector::DistSquared(Guard->GetActorLocation(), Worker->GetActorLocation());
            if (DistanceSq < BestDistanceSq)
            {
                BestDistanceSq = DistanceSq;
                NearestWorker = Worker;
            }
        }

        if (NearestWorker)
        {
            Guard->SetTargetActor(NearestWorker);
            NearestWorker->NotifyGuardPressure(Guard);
            ++Assigned;
        }
    }

    // Unassigned guards remain available to pressure the hero through their normal AI.
    if (Hero && Assigned == 0 && ResponseLevel > 0)
    {
        for (AALSAEQAEnemyCharacter* Guard : Guards)
        {
            if (IsValid(Guard) && Guard->GetEnemyState() != EALSAEQAEnemyState::Dead)
            {
                Guard->SetTargetActor(Hero);
            }
        }
    }
}
