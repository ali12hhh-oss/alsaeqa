#pragma once

#include "CoreMinimal.h"
#include "ALSAEQAStageFlowTypes.generated.h"

USTRUCT(BlueprintType)
struct FALSAEQAStageBeat
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName BeatId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName StageId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NextStageId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bPlayerControl = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bEndsStage = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bCanRepeatAfterProgress = true;
};

USTRUCT(BlueprintType)
struct FALSAEQAStageDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName StageId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 StageNumber = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NextStageId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RequiredPower = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RequiredWeapon = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FALSAEQAStageBeat> Interludes;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> PersistentWorldFlags;
};
