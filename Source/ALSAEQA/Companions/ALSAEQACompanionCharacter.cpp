#include "ALSAEQACompanionCharacter.h"

#include "Systems/ALSAEQAHealthComponent.h"
#include "Companions/ALSAEQACompanionStoryComponent.h"
#include "Companions/ALSAEQACompanionCaptureComponent.h"
#include "Companions/ALSAEQARidingComponent.h"
#include "Companions/ALSAEQAMountActor.h"
#include "Visual/ALSAEQAVisualAssetComponent.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Save/ALSAEQASaveManager.h"

AALSAEQACompanionCharacter::AALSAEQACompanionCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    StoryComponent = CreateDefaultSubobject<UALSAEQACompanionStoryComponent>(TEXT("StoryComponent"));
    CaptureComponent = CreateDefaultSubobject<UALSAEQACompanionCaptureComponent>(TEXT("CaptureComponent"));
    RidingComponent = CreateDefaultSubobject<UALSAEQARidingComponent>(TEXT("RidingComponent"));
    VisualAssetComponent = CreateDefaultSubobject<UALSAEQAVisualAssetComponent>(TEXT("VisualAssetComponent"));
    VisualAssetComponent->bApplyOnBeginPlay = true;
}

void AALSAEQACompanionCharacter::SetBehaviorState(EALSAEQACompanionBehaviorState NewState) { BehaviorState = NewState; }

void AALSAEQACompanionCharacter::SetCompanionId(FName NewCompanionId)
{
    if (!NewCompanionId.IsNone())
    {
        CompanionId = NewCompanionId;
        if (StoryComponent) StoryComponent->SetCompanionId(CompanionId);
    }
}

bool AALSAEQACompanionCharacter::CaptureCompanion()
{
    if (RidingComponent && RidingComponent->IsRiding()) RidingComponent->Dismount();
    if (!CaptureComponent || !CaptureComponent->Capture()) return false;
    bHelpingHeroFamilySearch = false;
    SetBehaviorState(EALSAEQACompanionBehaviorState::Captured);
    if (StoryComponent) StoryComponent->SetState(EALSAEQACompanionStoryState::Captured);
    return true;
}

bool AALSAEQACompanionCharacter::MarkRescueKnown()
{
    if (!CaptureComponent || !CaptureComponent->MarkRescueKnown()) return false;
    SetBehaviorState(EALSAEQACompanionBehaviorState::Captured);
    if (StoryComponent) StoryComponent->SetState(EALSAEQACompanionStoryState::RescueKnown);
    return true;
}

bool AALSAEQACompanionCharacter::RescueCompanion()
{
    if (!CaptureComponent || !CaptureComponent->Rescue()) return false;
    SetBehaviorState(EALSAEQACompanionBehaviorState::Rescued);
    if (StoryComponent) StoryComponent->RegisterRescue();
    return true;
}

bool AALSAEQACompanionCharacter::BeginFamilySearch()
{
    UWorld* World = GetWorld();
    UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;
    if (!SaveManager || !SaveManager->CanBeginFamilySearch()) return false;
    return StoryComponent && StoryComponent->SetState(EALSAEQACompanionStoryState::FamilyRevelation);
}

bool AALSAEQACompanionCharacter::BeginHelpingHeroFamilySearch()
{
    UWorld* World = GetWorld();
    UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
    UALSAEQASaveManager* SaveManager = GameInstance ? GameInstance->GetSubsystem<UALSAEQASaveManager>() : nullptr;
    if (!SaveManager || SaveManager->GetStage() < 40 || !CaptureComponent || !CaptureComponent->IsRescued()) return false;
    bHelpingHeroFamilySearch = true;
    SetBehaviorState(EALSAEQACompanionBehaviorState::Assisting);
    return true;
}

bool AALSAEQACompanionCharacter::Mount(AALSAEQAMountActor* MountActor)
{
    if (!MountActor || !RidingComponent || BehaviorState == EALSAEQACompanionBehaviorState::Captured || BehaviorState == EALSAEQACompanionBehaviorState::Separated) return false;
    return RidingComponent->TryMount(MountActor);
}

bool AALSAEQACompanionCharacter::Dismount() { return RidingComponent && RidingComponent->Dismount(); }
