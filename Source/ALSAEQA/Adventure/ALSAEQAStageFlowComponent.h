#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Adventure/ALSAEQAStageFlowTypes.h"
#include "ALSAEQAStageFlowComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FALSAEQAStageEvent, FName, StageId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FALSAEQAInterludeEvent, FName, BeatId);

UCLASS(ClassGroup=(ALSAEQA), meta=(BlueprintSpawnableComponent))
class ALSAEQA_API UALSAEQAStageFlowComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UALSAEQAStageFlowComponent();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Adventure") bool SetStage(const FALSAEQAStageDefinition& Definition);
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Adventure") bool AdvanceToNextStage();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Adventure") bool PlayNextInterlude();
    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Adventure") void MarkBeatSeen(FName BeatId);
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Adventure") FName GetCurrentStageId() const { return CurrentStage.StageId; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Adventure") int32 GetCurrentStageNumber() const { return CurrentStage.StageNumber; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Adventure") bool IsStageStillActive() const { return bStageActive; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Adventure") bool WasBeatSeen(FName BeatId) const { return SeenBeatIds.Contains(BeatId); }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Difficulty") float GetDifficultyRating() const { return CurrentStage.DifficultyRating; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Difficulty") float GetEnemyHealthMultiplier() const { return CurrentStage.EnemyHealthMultiplier; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Difficulty") float GetEnemyDamageMultiplier() const { return CurrentStage.EnemyDamageMultiplier; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Difficulty") float GetEnemyAggressionMultiplier() const { return CurrentStage.EnemyAggressionMultiplier; }
    UFUNCTION(BlueprintPure, Category="ALSAEQA|Difficulty") float GetHazardMultiplier() const { return CurrentStage.HazardMultiplier; }

    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Adventure") FALSAEQAStageEvent OnStageEntered;
    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Adventure") FALSAEQAStageEvent OnStageAdvanced;
    UPROPERTY(BlueprintAssignable, Category="ALSAEQA|Adventure") FALSAEQAInterludeEvent OnInterludeStarted;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ALSAEQA|Adventure", meta=(AllowPrivateAccess="true"))
    FALSAEQAStageDefinition CurrentStage;

    UPROPERTY(VisibleAnywhere, Category="ALSAEQA|Adventure")
    TSet<FName> SeenBeatIds;

    UPROPERTY(VisibleAnywhere, Category="ALSAEQA|Adventure")
    int32 NextInterludeIndex = 0;

    UPROPERTY(VisibleAnywhere, Category="ALSAEQA|Adventure")
    bool bStageActive = false;
};
