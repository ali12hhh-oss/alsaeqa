#include "Companions/ALSAEQACompanionStoryComponent.h"

#include "Engine/World.h"
#include "Save/ALSAEQASaveManager.h"

UALSAEQACompanionStoryComponent::UALSAEQACompanionStoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UALSAEQACompanionStoryComponent::SetState(EALSAEQACompanionStoryState NewState)
{
    UWorld* World = GetWorld();
    UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;

    if (SaveManager && !SaveManager->SetCompanionState(NewState))
    {
        return false;
    }

    Progress.State = NewState;
    OnStateChanged.Broadcast(NewState);
    return true;
}

bool UALSAEQACompanionStoryComponent::DiscoverFamilyClue(FName ClueId)
{
    if (ClueId.IsNone())
    {
        return false;
    }

    UWorld* World = GetWorld();
    UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;

    // Clues are intentionally allowed before stage 25; they never start the family search.
    if (SaveManager && SaveManager->AddFamilyClue(ClueId))
    {
        const FALSAEQACompanionStoryProgress SavedProgress = SaveManager->GetCompanionStory();
        Progress.FamilyClueCount = SavedProgress.FamilyClueCount;
        Progress.FamilyClueIds = SavedProgress.FamilyClueIds;
        Progress.State = SavedProgress.State;
        return true;
    }

    return false;
}

bool UALSAEQACompanionStoryComponent::DiscoverMemory(FName MemoryId)
{
    if (MemoryId.IsNone())
    {
        return false;
    }

    UWorld* World = GetWorld();
    UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;
    return !SaveManager || SaveManager->RegisterMemory(MemoryId);
}

bool UALSAEQACompanionStoryComponent::RegisterRescue()
{
    if (!SetState(EALSAEQACompanionStoryState::Rescued))
    {
        return false;
    }

    ++Progress.RescueCount;
    return true;
}

void UALSAEQACompanionStoryComponent::SetCompanionId(FName NewCompanionId)
{
    if (!NewCompanionId.IsNone())
    {
        Progress.CompanionId = NewCompanionId;
    }
}

bool UALSAEQACompanionStoryComponent::IsFamilySearchUnlocked() const
{
    const UWorld* World = GetWorld();
    const UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    const UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;
    return SaveManager && SaveManager->CanBeginFamilySearch();
}
