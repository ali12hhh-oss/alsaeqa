#include "Progression/ALSAEQAStageFlowComponent.h"

#include "Progression/ALSAEQAProgressionComponent.h"
#include "Progression/ALSAEQAProgressionStageRegistry.h"
#include "Save/ALSAEQASaveManager.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UALSAEQAStageFlowComponent::UALSAEQAStageFlowComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAStageFlowComponent::BeginPlay()
{
    Super::BeginPlay();
    Progression = GetOwner() ? GetOwner()->FindComponentByClass<UALSAEQAProgressionComponent>() : nullptr;

    if (!Progression || !GetWorld() || !GetWorld()->GetGameInstance())
    {
        return;
    }

    if (UALSAEQASaveManager* SaveManager = GetWorld()->GetGameInstance()->GetSubsystem<UALSAEQASaveManager>())
    {
        SaveManager->LoadProgress();
        const int32 SavedStage = SaveManager->GetStage();
        if (Progression->IsValidStage(SavedStage) && SavedStage > Progression->GetCurrentStage())
        {
            Progression->AdvanceStage(SavedStage);
        }
    }
}

bool UALSAEQAStageFlowComponent::CompleteCurrentStage()
{
    if (!Progression || bTransitionPending)
    {
        return false;
    }

    const int32 CurrentStage = Progression->GetCurrentStage();
    const FALSAEQAProgressionStage* Definition = ALSAEQAProgressionStageRegistry::FindStage(CurrentStage);
    if (!Definition || Definition->NextStage == INDEX_NONE)
    {
        return false;
    }

    const int32 NextStage = Definition->NextStage;
    if (!ALSAEQAProgressionStageRegistry::IsValidStage(NextStage))
    {
        return false;
    }

    PendingStage = NextStage;
    bTransitionPending = true;
    OnStageCompleted.Broadcast(CurrentStage);

    if (TransitionDelay <= 0.0f || !GetWorld())
    {
        ApplyPendingTransition();
        return true;
    }

    GetWorld()->GetTimerManager().SetTimer(
        TransitionTimerHandle,
        this,
        &UALSAEQAStageFlowComponent::ApplyPendingTransition,
        FMath::Max(0.05f, TransitionDelay),
        false);

    return true;
}

void UALSAEQAStageFlowComponent::CancelPendingTransition()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TransitionTimerHandle);
    }

    PendingStage = INDEX_NONE;
    bTransitionPending = false;
}

void UALSAEQAStageFlowComponent::ApplyPendingTransition()
{
    const int32 NextStage = PendingStage;
    PendingStage = INDEX_NONE;
    bTransitionPending = false;

    if (!Progression || !Progression->IsValidStage(NextStage))
    {
        return;
    }

    const int32 PreviousStage = Progression->GetCurrentStage();
    if (Progression->AdvanceStage(NextStage))
    {
        if (GetWorld() && GetWorld()->GetGameInstance())
        {
            if (UALSAEQASaveManager* SaveManager = GetWorld()->GetGameInstance()->GetSubsystem<UALSAEQASaveManager>())
            {
                SaveManager->SetStage(NextStage);
            }
        }

        OnAutomaticStageChanged.Broadcast(PreviousStage, NextStage);
    }
}
