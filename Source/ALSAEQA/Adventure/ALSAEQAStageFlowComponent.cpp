#include "Adventure/ALSAEQAStageFlowComponent.h"

namespace
{
    void ApplyMonotonicDifficulty(FALSAEQAStageDefinition& Stage)
    {
        const int32 Number = FMath::Max(1, Stage.StageNumber);
        const float Progress = static_cast<float>(Number - 1);

        // Every later stage receives a strictly higher baseline. Explicit designer
        // values may raise difficulty further, but can never lower this baseline.
        Stage.DifficultyRating = FMath::Max(Stage.DifficultyRating, FMath::Min(10.0f, 1.0f + Progress * 0.10f));
        Stage.EnemyHealthMultiplier = FMath::Max(Stage.EnemyHealthMultiplier, 1.0f + Progress * 0.015f);
        Stage.EnemyDamageMultiplier = FMath::Max(Stage.EnemyDamageMultiplier, 1.0f + Progress * 0.012f);
        Stage.EnemyAggressionMultiplier = FMath::Max(Stage.EnemyAggressionMultiplier, 1.0f + Progress * 0.010f);
        Stage.HazardMultiplier = FMath::Max(Stage.HazardMultiplier, 1.0f + Progress * 0.014f);
    }
}

UALSAEQAStageFlowComponent::UALSAEQAStageFlowComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UALSAEQAStageFlowComponent::SetStage(const FALSAEQAStageDefinition& Definition)
{
    if (Definition.StageId.IsNone() || Definition.StageNumber < 1)
    {
        return false;
    }

    CurrentStage = Definition;
    ApplyMonotonicDifficulty(CurrentStage);
    NextInterludeIndex = 0;
    bStageActive = true;
    OnStageEntered.Broadcast(CurrentStage.StageId);
    return true;
}

bool UALSAEQAStageFlowComponent::PlayNextInterlude()
{
    while (NextInterludeIndex < CurrentStage.Interludes.Num())
    {
        const FALSAEQAStageBeat& Beat = CurrentStage.Interludes[NextInterludeIndex++];
        if (Beat.BeatId.IsNone() || WasBeatSeen(Beat.BeatId))
        {
            continue;
        }

        OnInterludeStarted.Broadcast(Beat.BeatId);
        return true;
    }
    return false;
}

void UALSAEQAStageFlowComponent::MarkBeatSeen(FName BeatId)
{
    if (!BeatId.IsNone())
    {
        SeenBeatIds.Add(BeatId);
    }
}

bool UALSAEQAStageFlowComponent::AdvanceToNextStage()
{
    if (CurrentStage.NextStageId.IsNone())
    {
        return false;
    }

    const FName PreviousStage = CurrentStage.StageId;
    CurrentStage.StageId = CurrentStage.NextStageId;
    CurrentStage.StageNumber = FMath::Max(1, CurrentStage.StageNumber + 1);
    ApplyMonotonicDifficulty(CurrentStage);
    NextInterludeIndex = 0;
    bStageActive = true;
    OnStageAdvanced.Broadcast(CurrentStage.StageId);
    OnStageEntered.Broadcast(CurrentStage.StageId);
    return PreviousStage != CurrentStage.StageId;
}
