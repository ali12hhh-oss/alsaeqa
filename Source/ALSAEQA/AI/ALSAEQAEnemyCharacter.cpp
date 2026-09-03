#include "AI/ALSAEQAEnemyCharacter.h"
#include "Systems/ALSAEQAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AALSAEQAEnemyCharacter::AALSAEQAEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}

void AALSAEQAEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &AALSAEQAEnemyCharacter::HandleDeath);
    }
}

void AALSAEQAEnemyCharacter::SetEnemyState(EALSAEQAEnemyState NewState)
{
    EnemyState = NewState;
    if (NewState == EALSAEQAEnemyState::Dead)
    {
        GetCharacterMovement()->DisableMovement();
        SetActorEnableCollision(false);
    }
}

void AALSAEQAEnemyCharacter::SetTargetActor(AActor* NewTarget)
{
    TargetActor = NewTarget;
    if (NewTarget)
    {
        SetEnemyState(EALSAEQAEnemyState::Alert);
    }
}

float AALSAEQAEnemyCharacter::ReceiveALSAEQADamage_Implementation(const FALSAEQADamageInfo& DamageInfo)
{
    if (!HealthComponent || HealthComponent->IsDead() || DamageInfo.Amount <= 0.0f)
    {
        return 0.0f;
    }

    HealthComponent->ApplyDamage(DamageInfo.Amount);
    if (!HealthComponent->IsDead() && EnemyState != EALSAEQAEnemyState::Stunned)
    {
        SetEnemyState(EALSAEQAEnemyState::Alert);
        if (DamageInfo.Instigator)
        {
            SetTargetActor(DamageInfo.Instigator);
        }
    }
    return DamageInfo.Amount;
}

void AALSAEQAEnemyCharacter::HandleDeath()
{
    SetEnemyState(EALSAEQAEnemyState::Dead);
}
