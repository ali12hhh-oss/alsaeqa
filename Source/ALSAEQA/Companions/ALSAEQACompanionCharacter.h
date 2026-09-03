#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Companions/ALSAEQACompanionStoryTypes.h"
#include "ALSAEQACompanionCharacter.generated.h"

class UALSAEQAHealthComponent;
class UALSAEQACompanionStoryComponent;
class UALSAEQACompanionCaptureComponent;

UENUM(BlueprintType)
enum class EALSAEQACompanionBehaviorState : uint8
{
    Following,
    Assisting,
    Separated,
    Captured,
    Rescued
};

UCLASS(BlueprintType, Blueprintable)
class ALSAEQA_API AALSAEQACompanionCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AALSAEQACompanionCharacter();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    void SetBehaviorState(EALSAEQACompanionBehaviorState NewState);

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    void SetCompanionId(FName NewCompanionId);

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    bool CaptureCompanion();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    bool MarkRescueKnown();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    bool RescueCompanion();

    UFUNCTION(BlueprintCallable, Category="ALSAEQA|Companion")
    bool BeginFamilySearch();

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Companion")
    FName GetCompanionId() const { return CompanionId; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Companion")
    EALSAEQACompanionBehaviorState GetBehaviorState() const { return BehaviorState; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Companion")
    UALSAEQACompanionStoryComponent* GetStoryComponent() const { return StoryComponent; }

    UFUNCTION(BlueprintPure, Category="ALSAEQA|Companion")
    UALSAEQACompanionCaptureComponent* GetCaptureComponent() const { return CaptureComponent; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Companion")
    TObjectPtr<UALSAEQAHealthComponent> HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Companion")
    TObjectPtr<UALSAEQACompanionStoryComponent> StoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ALSAEQA|Companion")
    TObjectPtr<UALSAEQACompanionCaptureComponent> CaptureComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="ALSAEQA|Companion")
    FName CompanionId = TEXT("PermanentCompanion");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Companion")
    float FollowDistance = 220.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ALSAEQA|Companion")
    float AssistRange = 900.0f;

    UPROPERTY(BlueprintReadOnly, Category="ALSAEQA|Companion")
    EALSAEQACompanionBehaviorState BehaviorState = EALSAEQACompanionBehaviorState::Following;
};
