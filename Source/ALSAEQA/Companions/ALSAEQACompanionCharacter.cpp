#include "Companions/ALSAEQACompanionCharacter.h"

#include "Systems/ALSAEQAHealthComponent.h"
#include "Companions/ALSAEQACompanionStoryComponent.h"
#include "Companions/ALSAEQACompanionCaptureComponent.h"

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
            StoryComponent->GetProgress().CompanionId = CompanionId;
        }
    }
}
