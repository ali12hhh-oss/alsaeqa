#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Companions/ALSAEQACompanionStoryTypes.h"
#include "Save/ALSAEQASaveGame.h"
#include "ALSAEQASaveManager.generated.h"

UCLASS()
class ALSAEQA_API UALSAEQASaveManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Save")
    bool SaveProgress();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Save")
    bool LoadProgress();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Save")
    void SetStage(int32 NewStage);

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Save")
    bool SaveCheckpoint(FName CheckpointId, FName RegionId, int32 Stage, FVector PlayerLocation, FRotator PlayerRotation);

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Save")
    int32 GetStage() const;

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Save")
    FALSAEQACheckpointData GetLastCheckpoint() const;

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Legacy")
    bool RegisterMemory(FName MemoryId);

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Legacy")
    bool RegisterWorldFlag(FName FlagId);

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Legacy")
    bool HasMemory(FName MemoryId) const;

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Legacy")
    bool HasWorldFlag(FName FlagId) const;

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    bool AddFamilyClue(FName ClueId);

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    bool CanBeginFamilySearch() const;

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Companion")
    FALSAEQACompanionStoryProgress GetCompanionStory() const { return SaveData.CompanionStory; }

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    bool SetCompanionState(EALSAEQACompanionStoryState NewState);

private:
    static const FString SaveSlotName;
    static constexpr int32 FamilySearchStage = 25;
    UPROPERTY()
    UALSAEQASaveGame* SaveData = nullptr;

    bool EnsureSaveData();
};
