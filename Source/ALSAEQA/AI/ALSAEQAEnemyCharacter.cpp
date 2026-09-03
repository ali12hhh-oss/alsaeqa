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
        HealthComponent->OnDeath.AddDynamic(this, &AALSAEQAEnemyCharacter::SetEnemyState);
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
