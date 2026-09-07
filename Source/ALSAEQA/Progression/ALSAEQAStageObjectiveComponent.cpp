#include "Progression/ALSAEQAStageObjectiveComponent.h"

#include "Progression/ALSAEQAProgressionComponent.h"
#include "Progression/ALSAEQAStageFlowComponent.h"
#include "Story/ALSAEQALegacyComponent.h"

UALSAEQAStageObjectiveComponent::UALSAEQAStageObjectiveComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAStageObjectiveComponent::BeginPlay()
{
    Super::BeginPlay();
    StageFlow = GetOwner() ? GetOwner()->FindComponentByClass<UALSAEQAStageFlowComponent>() : nullptr;

    if (ObjectiveRequirements.Num() == 0)
    {
        int32 StageNumber = 1;
        if (UALSAEQAProgressionComponent* Progression = GetOwner() ? GetOwner()->FindComponentByClass<UALSAEQAProgressionComponent>() : nullptr)
        {
            StageNumber = Progression->GetCurrentStage();
        }
        if (StageNumber == 1)
        {
            ObjectiveRequirements.Add(TEXT("RescueWorkers"), 5);
            ObjectiveRequirements.Add(TEXT("DefeatSlavers"), 1);
        }
    }

    ResetObjectives();
}

void UALSAEQAStageObjectiveComponent::ResetObjectives()
{
    ObjectiveProgress.Reset();
    for (const TPair<FName, int32>& Pair : ObjectiveRequirements)
    {
        if (!Pair.Key.IsNone() && Pair.Value > 0) ObjectiveProgress.Add(Pair.Key, 0);
    }
}

int32 UALSAEQAStageObjectiveComponent::GetObjectiveProgress(FName ObjectiveId) const
{
    if (const int32* Value = ObjectiveProgress.Find(ObjectiveId)) return *Value;
    return 0;
}

bool UALSAEQAStageObjectiveComponent::IsObjectiveComplete(FName ObjectiveId) const
{
    const int32* Requirement = ObjectiveRequirements.Find(ObjectiveId);
    return Requirement && *Requirement > 0 && GetObjectiveProgress(ObjectiveId) >= *Requirement;
}

int32 UALSAEQAStageObjectiveComponent::GetCompletedObjectiveCount() const
{
    int32 Count = 0;
    for (const TPair<FName, int32>& Pair : ObjectiveRequirements)
    {
        if (Pair.Value > 0 && IsObjectiveComplete(Pair.Key)) ++Count;
    }
    return Count;
}

bool UALSAEQAStageObjectiveComponent::AreAllObjectivesComplete() const
{
    if (ObjectiveRequirements.Num() == 0) return false;
    for (const TPair<FName, int32>& Pair : ObjectiveRequirements)
    {
        if (Pair.Value <= 0 || !IsObjectiveComplete(Pair.Key)) return false;
    }
    return true;
}

bool UALSAEQAStageObjectiveComponent::RegisterProgress(FName ObjectiveId, int32 Amount)
{
    if (ObjectiveId.IsNone() || Amount <= 0 || !ObjectiveRequirements.Contains(ObjectiveId) || IsObjectiveComplete(ObjectiveId)) return false;

    const int32 Requirement = ObjectiveRequirements.FindChecked(ObjectiveId);
    const int32 NewProgress = FMath::Clamp(GetObjectiveProgress(ObjectiveId) + Amount, 0, Requirement);
    ObjectiveProgress.FindOrAdd(ObjectiveId) = NewProgress;
    OnObjectiveProgress.Broadcast(ObjectiveId, NewProgress);

    if (NewProgress >= Requirement) OnObjectiveCompleted.Broadcast(ObjectiveId);

    FinalizeStageIfReady();
    return true;
}

bool UALSAEQAStageObjectiveComponent::CompleteObjective(FName ObjectiveId)
{
    if (!ObjectiveRequirements.Contains(ObjectiveId)) return false;

    const int32 Requirement = FMath::Max(1, ObjectiveRequirements.FindChecked(ObjectiveId));
    const int32 Current = GetObjectiveProgress(ObjectiveId);
    if (Current >= Requirement) return false;

    return RegisterProgress(ObjectiveId, Requirement - Current);
}

bool UALSAEQAStageObjectiveComponent::FinalizeStageIfReady()
{
    if (!StageFlow || !AreAllObjectivesComplete()) return false;

    // Stage 1's first clue is unlocked exactly once at the moment both
    // mandatory gameplay gates are satisfied. It is a story discovery, not
    // an extra gate, so the automatic stage transition cannot deadlock.
    if (AActor* Owner = GetOwner())
    {
        if (UALSAEQALegacyComponent* Legacy = Owner->FindComponentByClass<UALSAEQALegacyComponent>())
        {
            Legacy->AddFamilyClue(TEXT("Stage1_MineNetwork"));
            Legacy->SetLegacyFlag(TEXT("Stage1_FirstClueFound"), true);
        }
    }

    return StageFlow->CompleteCurrentStage();
}
