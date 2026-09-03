#include "Adventure/ALSAEQAStageFlowComponent.h"

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

        // These beats intentionally do not end the stage. Blueprint/cinematic
        // systems can pause player input while companions talk or an event plays.
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
    NextInterludeIndex = 0;
    bStageActive = true;
    OnStageAdvanced.Broadcast(CurrentStage.StageId);
    OnStageEntered.Broadcast(CurrentStage.StageId);
    return PreviousStage != CurrentStage.StageId;
}
