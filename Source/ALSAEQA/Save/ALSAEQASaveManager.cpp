#include "Save/ALSAEQASaveManager.h"

#include "Kismet/GameplayStatics.h"

const FString UALSAEQASaveManager::SaveSlotName(TEXT("ALSAEQA_Main"));

bool UALSAEQASaveManager::EnsureSaveData()
{
    if (SaveData)
    {
        return true;
    }

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        SaveData = Cast<UALSAEQASaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }

    if (!SaveData)
    {
        SaveData = Cast<UALSAEQASaveGame>(UGameplayStatics::CreateSaveGameObject(UALSAEQASaveGame::StaticClass()));
    }

    return SaveData != nullptr;
}

bool UALSAEQASaveManager::SaveProgress()
{
    return EnsureSaveData() && UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, 0);
}

bool UALSAEQASaveManager::LoadProgress()
{
    SaveData = nullptr;
    return EnsureSaveData();
}

void UALSAEQASaveManager::SetStage(int32 NewStage)
{
    if (!EnsureSaveData())
    {
        return;
    }

    SaveData->LastCheckpoint.Stage = FMath::Max(0, NewStage);
}

bool UALSAEQASaveManager::SaveCheckpoint(FName CheckpointId, FName RegionId, int32 Stage, FVector PlayerLocation, FRotator PlayerRotation)
{
    if (!EnsureSaveData() || CheckpointId.IsNone())
    {
        return false;
    }

    SaveData->LastCheckpoint.CheckpointId = CheckpointId;
    SaveData->LastCheckpoint.RegionId = RegionId;
    SaveData->LastCheckpoint.Stage = FMath::Max(0, Stage);
    SaveData->LastCheckpoint.PlayerLocation = PlayerLocation;
    SaveData->LastCheckpoint.PlayerRotation = PlayerRotation;
    return SaveProgress();
}

int32 UALSAEQASaveManager::GetStage() const
{
    return SaveData ? SaveData->LastCheckpoint.Stage : 0;
}

FALSAEQACheckpointData UALSAEQASaveManager::GetLastCheckpoint() const
{
    return SaveData ? SaveData->LastCheckpoint : FALSAEQACheckpointData();
}

bool UALSAEQASaveManager::RegisterMemory(FName MemoryId)
{
    if (!EnsureSaveData() || MemoryId.IsNone() || SaveData->DiscoveredMemoryIds.Contains(MemoryId))
    {
        return false;
    }

    SaveData->DiscoveredMemoryIds.Add(MemoryId);
    return true;
}

bool UALSAEQASaveManager::RegisterWorldFlag(FName FlagId)
{
    if (!EnsureSaveData() || FlagId.IsNone() || SaveData->PersistentWorldFlags.Contains(FlagId))
    {
        return false;
    }

    SaveData->PersistentWorldFlags.Add(FlagId);
    return true;
}

bool UALSAEQASaveManager::HasMemory(FName MemoryId) const
{
    return SaveData && !MemoryId.IsNone() && SaveData->DiscoveredMemoryIds.Contains(MemoryId);
}

bool UALSAEQASaveManager::HasWorldFlag(FName FlagId) const
{
    return SaveData && !FlagId.IsNone() && SaveData->PersistentWorldFlags.Contains(FlagId);
}

bool UALSAEQASaveManager::AddFamilyClue(FName ClueId)
{
    if (!EnsureSaveData() || ClueId.IsNone() || SaveData->CompanionStory.FamilyClueIds.Contains(ClueId))
    {
        return false;
    }

    SaveData->CompanionStory.FamilyClueIds.Add(ClueId);
    SaveData->CompanionStory.FamilyClueCount = SaveData->CompanionStory.FamilyClueIds.Num();
    if (SaveData->CompanionStory.State < EALSAEQACompanionStoryState::FamilyClue)
    {
        SaveData->CompanionStory.State = EALSAEQACompanionStoryState::FamilyClue;
    }
    return true;
}

bool UALSAEQASaveManager::HasFamilyClue(FName ClueId) const
{
    return SaveData && !ClueId.IsNone() && SaveData->CompanionStory.FamilyClueIds.Contains(ClueId);
}

bool UALSAEQASaveManager::CanBeginFamilySearch() const
{
    return SaveData && SaveData->LastCheckpoint.Stage >= FamilySearchStage;
}

bool UALSAEQASaveManager::SetCompanionState(EALSAEQACompanionStoryState NewState)
{
    if (!EnsureSaveData())
    {
        return false;
    }

    if (NewState == EALSAEQACompanionStoryState::FamilyRevelation && !CanBeginFamilySearch())
    {
        return false;
    }

    SaveData->CompanionStory.State = NewState;
    return true;
}
