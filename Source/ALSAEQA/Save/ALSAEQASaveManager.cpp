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
    if (!EnsureSaveData()) return;
    SaveData->LastCheckpoint.Stage = FMath::Max(0, NewStage);
    if (NewStage >= FamilySearchStartStage)
    {
        SaveData->CompanionStory.FamilySearchStage = FMath::Max(SaveData->CompanionStory.FamilySearchStage, FMath::Min(NewStage, FamilySearchEndStage));
    }
    if (NewStage >= FamilySearchEndStage)
    {
        SaveData->CompanionStory.FamilySearchStage = FamilySearchEndStage;
        SaveData->CompanionStory.bFamilySearchCompleted = true;
    }
}

bool UALSAEQASaveManager::SaveCheckpoint(FName CheckpointId, FName RegionId, int32 Stage, FVector PlayerLocation, FRotator PlayerRotation)
{
    if (!EnsureSaveData() || CheckpointId.IsNone()) return false;
    SaveData->LastCheckpoint.CheckpointId = CheckpointId;
    SaveData->LastCheckpoint.RegionId = RegionId;
    SaveData->LastCheckpoint.Stage = FMath::Max(0, Stage);
    SaveData->LastCheckpoint.PlayerLocation = PlayerLocation;
    SaveData->LastCheckpoint.PlayerRotation = PlayerRotation;
    if (Stage >= FamilySearchStartStage)
    {
        SaveData->CompanionStory.FamilySearchStage = FMath::Max(SaveData->CompanionStory.FamilySearchStage, FMath::Min(Stage, FamilySearchEndStage));
    }
    if (Stage >= FamilySearchEndStage)
    {
        SaveData->CompanionStory.FamilySearchStage = FamilySearchEndStage;
        SaveData->CompanionStory.bFamilySearchCompleted = true;
    }
    return SaveProgress();
}

int32 UALSAEQASaveManager::GetStage() const { return SaveData ? SaveData->LastCheckpoint.Stage : 0; }
FALSAEQACheckpointData UALSAEQASaveManager::GetLastCheckpoint() const { return SaveData ? SaveData->LastCheckpoint : FALSAEQACheckpointData(); }

bool UALSAEQASaveManager::RegisterMemory(FName MemoryId)
{
    if (!EnsureSaveData() || MemoryId.IsNone() || SaveData->DiscoveredMemoryIds.Contains(MemoryId)) return false;
    SaveData->DiscoveredMemoryIds.Add(MemoryId);
    return true;
}

bool UALSAEQASaveManager::RegisterWorldFlag(FName FlagId)
{
    if (!EnsureSaveData() || FlagId.IsNone() || SaveData->PersistentWorldFlags.Contains(FlagId)) return false;
    SaveData->PersistentWorldFlags.Add(FlagId);
    return true;
}

bool UALSAEQASaveManager::HasMemory(FName MemoryId) const { return SaveData && !MemoryId.IsNone() && SaveData->DiscoveredMemoryIds.Contains(MemoryId); }
bool UALSAEQASaveManager::HasWorldFlag(FName FlagId) const { return SaveData && !FlagId.IsNone() && SaveData->PersistentWorldFlags.Contains(FlagId); }

bool UALSAEQASaveManager::AddFamilyClue(FName ClueId)
{
    if (!EnsureSaveData() || ClueId.IsNone() || SaveData->CompanionStory.FamilyClueIds.Contains(ClueId)) return false;
    SaveData->CompanionStory.FamilyClueIds.Add(ClueId);
    SaveData->CompanionStory.FamilyClueCount = SaveData->CompanionStory.FamilyClueIds.Num();
    if (SaveData->CompanionStory.State < EALSAEQACompanionStoryState::FamilyClue) SaveData->CompanionStory.State = EALSAEQACompanionStoryState::FamilyClue;
    return true;
}

bool UALSAEQASaveManager::HasFamilyClue(FName ClueId) const { return SaveData && !ClueId.IsNone() && SaveData->CompanionStory.FamilyClueIds.Contains(ClueId); }

bool UALSAEQASaveManager::RecordFamilyEvidence(FName EvidenceId)
{
    if (!EnsureSaveData() || EvidenceId.IsNone() || !IsFamilySearchActive() || SaveData->CompanionStory.FamilyEvidenceIds.Contains(EvidenceId)) return false;
    SaveData->CompanionStory.FamilyEvidenceIds.Add(EvidenceId);
    SaveData->CompanionStory.FamilyEvidenceCount = SaveData->CompanionStory.FamilyEvidenceIds.Num();
    SaveData->CompanionStory.State = EALSAEQACompanionStoryState::FamilyClue;
    return SaveProgress();
}

bool UALSAEQASaveManager::HasFamilyEvidence(FName EvidenceId) const { return SaveData && !EvidenceId.IsNone() && SaveData->CompanionStory.FamilyEvidenceIds.Contains(EvidenceId); }

bool UALSAEQASaveManager::SetFamilySearchStage(int32 NewStage)
{
    if (!EnsureSaveData() || NewStage < FamilySearchStartStage || NewStage > FamilySearchEndStage || NewStage < SaveData->CompanionStory.FamilySearchStage) return false;
    SaveData->CompanionStory.FamilySearchStage = NewStage;
    if (NewStage >= FamilySearchEndStage) SaveData->CompanionStory.bFamilySearchCompleted = true;
    return SaveProgress();
}

bool UALSAEQASaveManager::IsFamilySearchActive() const
{
    if (!SaveData) return false;
    const int32 Stage = SaveData->LastCheckpoint.Stage;
    return Stage >= FamilySearchStartStage && Stage <= FamilySearchEndStage;
}

int32 UALSAEQASaveManager::GetFamilySearchStage() const { return SaveData ? SaveData->CompanionStory.FamilySearchStage : 0; }
int32 UALSAEQASaveManager::GetFamilyEvidenceCount() const { return SaveData ? SaveData->CompanionStory.FamilyEvidenceCount : 0; }
bool UALSAEQASaveManager::IsFamilySearchCompleted() const { return SaveData && SaveData->CompanionStory.bFamilySearchCompleted; }
bool UALSAEQASaveManager::CanBeginFamilySearch() const { return SaveData && SaveData->LastCheckpoint.Stage >= FamilySearchStartStage; }

bool UALSAEQASaveManager::SetCompanionState(EALSAEQACompanionStoryState NewState)
{
    if (!EnsureSaveData()) return false;
    if (NewState == EALSAEQACompanionStoryState::FamilyRevelation && !CanBeginFamilySearch()) return false;
    SaveData->CompanionStory.State = NewState;
    return true;
}

bool UALSAEQASaveManager::SaveTamedMount(const FALSAEQAMountProfile& Profile)
{
    if (!EnsureSaveData() || Profile.MountId.IsNone() || Profile.TamingProgress < Profile.TamingRequired) return false;
    const int32 Existing = SaveData->TamedMountProfiles.IndexOfByPredicate([&Profile](const FALSAEQAMountProfile& Item){ return Item.MountId == Profile.MountId; });
    if (Existing == INDEX_NONE) SaveData->TamedMountProfiles.Add(Profile);
    else SaveData->TamedMountProfiles[Existing] = Profile;
    SaveData->TamedMountProfiles[Existing == INDEX_NONE ? SaveData->TamedMountProfiles.Num() - 1 : Existing].TamingProgress = FMath::Max(Profile.TamingProgress, Profile.TamingRequired);
    return SaveProgress();
}

bool UALSAEQASaveManager::HasTamedMount(FName MountId) const
{
    return SaveData && !MountId.IsNone() && SaveData->TamedMountProfiles.ContainsByPredicate([MountId](const FALSAEQAMountProfile& Profile){ return Profile.MountId == MountId; });
}

FALSAEQAMountProfile UALSAEQASaveManager::GetTamedMount(FName MountId) const
{
    if (SaveData && !MountId.IsNone())
    {
        const FALSAEQAMountProfile* Found = SaveData->TamedMountProfiles.FindByPredicate([MountId](const FALSAEQAMountProfile& Profile){ return Profile.MountId == MountId; });
        if (Found) return *Found;
    }
    return FALSAEQAMountProfile();
}
