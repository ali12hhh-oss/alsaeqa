#include "Companions/ALSAEQACompanionCharacter.h"

#include "Systems/ALSAEQAHealthComponent.h"
#include "Companions/ALSAEQACompanionStoryComponent.h"
#include "Companions/ALSAEQACompanionCaptureComponent.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Save/ALSAEQASaveManager.h"

AALSAEQACompanionCharacter::AALSAEQACompanionCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    StoryComponent = CreateDefaultSubobject<UALSAEQACompanionStoryComponent>(TEXT("StoryComponent"));
    CaptureComponent = CreateDefaultSubobject<UALSAEQACompanionCaptureComponent>(TEXT("CaptureComponent"));
}

void AALSAEQACompanionCharacter::SetBehaviorState(EALSAEQACompanionBehaviorState NewState)
{
    BehaviorState = NewState;
}

void AALSAEQACompanionCharacter::SetCompanionId(FName NewCompanionId)
{
    if (!NewCompanionId.IsNone())
    {
        CompanionId = NewCompanionId;
        if (StoryComponent)
        {
            StoryComponent->SetCompanionId(CompanionId);
        }
    }
}

bool AALSAEQACompanionCharacter::CaptureCompanion()
{
    if (!CaptureComponent || !CaptureComponent->Capture())
    {
        return false;
    }

    SetBehaviorState(EALSAEQACompanionBehaviorState::Captured);
    if (StoryComponent)
    {
        StoryComponent->SetState(EALSAEQACompanionStoryState::Captured);
    }
    return true;
}

bool AALSAEQACompanionCharacter::MarkRescueKnown()
{
    if (!CaptureComponent || !CaptureComponent->MarkRescueKnown())
    {
        return false;
    }

    SetBehaviorState(EALSAEQACompanionBehaviorState::Captured);
    if (StoryComponent)
    {
        StoryComponent->SetState(EALSAEQACompanionStoryState::RescueKnown);
    }
    return true;
}

bool AALSAEQACompanionCharacter::RescueCompanion()
{
    if (!CaptureComponent || !CaptureComponent->Rescue())
    {
        return false;
    }

    SetBehaviorState(EALSAEQACompanionBehaviorState::Rescued);
    if (StoryComponent)
    {
        StoryComponent->RegisterRescue();
    }
    return true;
}

bool AALSAEQACompanionCharacter::BeginFamilySearch()
{
    UWorld* World = GetWorld();
    UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;

    if (!SaveManager || !SaveManager->CanBeginFamilySearch())
    {
        return false;
    }

    return StoryComponent && StoryComponent->SetState(EALSAEQACompanionStoryState::FamilyRevelation);
}
