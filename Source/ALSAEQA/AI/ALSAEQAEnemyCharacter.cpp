#include "AI/ALSAEQAEnemyCharacter.h"
#include "Systems/ALSAEQAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Player/ALSAEQACharacter.h"

AALSAEQAEnemyCharacter::AALSAEQAEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}

void AALSAEQAEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (HealthComponent) HealthComponent->OnDeath.AddDynamic(this, &AALSAEQAEnemyCharacter::HandleDeath);
}

void AALSAEQAEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (EnemyState == EALSAEQAEnemyState::Dead || !GetWorld()) return;

    AActor* Target = TargetActor.Get();
    if (!IsValid(Target))
    {
        for (TActorIterator<AALSAEQACharacter> It(GetWorld()); It; ++It)
        {
            AALSAEQACharacter* Candidate = *It;
            if (IsValid(Candidate) && !Candidate->GetHealthComponent()->IsDead() && FVector::DistSquared(GetActorLocation(), Candidate->GetActorLocation()) <= FMath::Square(DetectionRange))
            {
                Target = Candidate;
                SetTargetActor(Target);
                break;
            }
        }
    }

    if (!IsValid(Target))
    {
        SetEnemyState(EALSAEQAEnemyState::Idle);
        return;
    }

    const float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
    if (Distance > DetectionRange)
    {
        TargetActor.Reset();
        SetEnemyState(EALSAEQAEnemyState::Idle);
        return;
    }

    if (Distance <= AttackRange)
    {
        SetEnemyState(EALSAEQAEnemyState::Attack);
        return;
    }

    SetEnemyState(EALSAEQAEnemyState::Chase);
    const FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
    AddMovementInput(Direction, 1.0f);
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
    if (NewTarget) SetEnemyState(EALSAEQAEnemyState::Alert);
}

float AALSAEQAEnemyCharacter::ReceiveALSAEQADamage_Implementation(const FALSAEQADamageInfo& DamageInfo)
{
    if (!HealthComponent || HealthComponent->IsDead() || DamageInfo.Amount <= 0.0f) return 0.0f;
    HealthComponent->ApplyDamage(DamageInfo.Amount);
    if (!HealthComponent->IsDead())
    {
        SetEnemyState(EALSAEQAEnemyState::Alert);
        if (DamageInfo.Instigator) SetTargetActor(DamageInfo.Instigator);
    }
    return DamageInfo.Amount;
}

void AALSAEQAEnemyCharacter::HandleDeath() { SetEnemyState(EALSAEQAEnemyState::Dead); }
