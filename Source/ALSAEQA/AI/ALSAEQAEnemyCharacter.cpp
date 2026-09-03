#include "AI/ALSAEQAEnemyCharacter.h"
#include "Systems/ALSAEQAHealthComponent.h"
#include "Visual/ALSAEQAVisualAssetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Player/ALSAEQACharacter.h"
#include "Save/ALSAEQASaveManager.h"
#include "Engine/GameInstance.h"

AALSAEQAEnemyCharacter::AALSAEQAEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    VisualAssetComponent = CreateDefaultSubobject<UALSAEQAVisualAssetComponent>(TEXT("VisualAssetComponent"));
    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}

void AALSAEQAEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (HealthComponent) HealthComponent->OnDeath.AddDynamic(this, &AALSAEQAEnemyCharacter::HandleDeath);
    AttackCooldownRemaining = 0.0f;
}

void AALSAEQAEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (EnemyState == EALSAEQAEnemyState::Dead || !GetWorld()) return;

    AttackCooldownRemaining = FMath::Max(0.0f, AttackCooldownRemaining - DeltaSeconds);

    int32 CurrentStage = 1;
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UALSAEQASaveManager* SaveManager = GameInstance->GetSubsystem<UALSAEQASaveManager>())
        {
            CurrentStage = FMath::Max(1, SaveManager->GetStage());
        }
    }

    const float Progress = static_cast<float>(CurrentStage - 1);
    const float StageSpeedMultiplier = 1.0f + Progress * 0.004f;
    const float StageDetectionMultiplier = 1.0f + Progress * 0.0025f;
    const float StageDamageMultiplier = 1.0f + Progress * 0.012f;
    const float StageAttackRateMultiplier = 1.0f + Progress * 0.003f;
    const float EffectiveDetectionRange = DetectionRange * StageDetectionMultiplier;
    const float EffectiveAttackRange = AttackRange * (1.0f + Progress * 0.0015f);
    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed * StageSpeedMultiplier;

    AActor* Target = TargetActor.Get();
    if (!IsValid(Target))
    {
        for (TActorIterator<AALSAEQACharacter> It(GetWorld()); It; ++It)
        {
            AALSAEQACharacter* Candidate = *It;
            if (IsValid(Candidate) && Candidate->GetHealthComponent() && !Candidate->GetHealthComponent()->IsDead() &&
                FVector::DistSquared(GetActorLocation(), Candidate->GetActorLocation()) <= FMath::Square(EffectiveDetectionRange))
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
    if (Distance > EffectiveDetectionRange)
    {
        TargetActor.Reset();
        SetEnemyState(EALSAEQAEnemyState::Idle);
        return;
    }

    if (Distance <= EffectiveAttackRange)
    {
        SetEnemyState(EALSAEQAEnemyState::Attack);
        AALSAEQACharacter* Player = Cast<AALSAEQACharacter>(Target);
        if (Player && Player->GetHealthComponent() && !Player->GetHealthComponent()->IsDead() && AttackCooldownRemaining <= 0.0f)
        {
            Player->GetHealthComponent()->ApplyDamage(AttackDamage * StageDamageMultiplier);
            AttackCooldownRemaining = AttackCooldown / StageAttackRateMultiplier;
        }
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
