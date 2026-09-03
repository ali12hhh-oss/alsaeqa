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
    UWorld* World = GetWorld();
    UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;

    // Clues may be discovered before stage 25, but they never start the family search.
    if (SaveManager && SaveManager->AddFamilyClue(ClueId))
    {
        Progress.FamilyClueCount = SaveManager->GetCompanionStory().FamilyClueCount;
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
    ++Progress.RescueCount;
    return SetState(EALSAEQACompanionStoryState::Rescued);
}

bool UALSAEQACompanionStoryComponent::IsFamilySearchUnlocked() const
{
    const UWorld* World = GetWorld();
    const UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    const UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;
    return SaveManager && SaveManager->CanBeginFamilySearch();
}
