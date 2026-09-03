#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/ALSAEQACheckpointTypes.h"
#include "ALSAEQASaveGame.generated.h"

UCLASS()
class ALSAEQA_API UALSAEQASaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, SaveGame, Category="ALSAEQA|Save")
    FALSAEQACheckpointData LastCheckpoint;

    UPROPERTY(BlueprintReadWrite, SaveGame, Category="ALSAEQA|Save")
    TArray<FName> CompletedDiscoveryIds;

    UPROPERTY(BlueprintReadWrite, SaveGame, Category="ALSAEQA|Save")
    TArray<uint8> UnlockedPowers;

    UPROPERTY(BlueprintReadWrite, SaveGame, Category="ALSAEQA|Save")
    TArray<uint8> AcquiredWeapons;
};
