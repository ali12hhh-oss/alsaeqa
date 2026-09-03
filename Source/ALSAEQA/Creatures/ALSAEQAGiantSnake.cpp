#include "Creatures/ALSAEQAGiantSnake.h"

#include "Systems/ALSAEQAHealthComponent.h"
#include "Combat/ALSAEQAResistanceComponent.h"

AALSAEQAGiantSnake::AALSAEQAGiantSnake()
{
    PrimaryActorTick.bCanEverTick = false;
    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    ResistanceComponent = CreateDefaultSubobject<UALSAEQAResistanceComponent>(TEXT("ResistanceComponent"));
}

float AALSAEQAGiantSnake::ReceiveALSAEQADamage_Implementation(const FALSAEQADamageInfo& DamageInfo)
{
    if (!HealthComponent || DamageInfo.Amount <= 0.0f)
    {
        return 0.0f;
    }

    const float FinalDamage = ResistanceComponent
        ? ResistanceComponent->ModifyDamage(DamageInfo)
        : DamageInfo.Amount;

    HealthComponent->ApplyDamage(FinalDamage);
    return FinalDamage;
}

void AALSAEQAGiantSnake::StartAmbush()
{
    // Animation/AI state is intentionally driven from Blueprint or the future AI controller.
}

void AALSAEQAGiantSnake::CoilAroundObstacle()
{
    // Used by level scripting to make the snake a traversal obstacle as well as an enemy.
}

void AALSAEQAGiantSnake::Retreat()
{
    // Retreat behavior is supplied by the AI controller/navigation layer.
}

bool AALSAEQAGiantSnake::IsDefeated() const
{
    return !HealthComponent || HealthComponent->IsDead();
}
