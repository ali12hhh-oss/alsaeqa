#include "Boss/ALSAEQABossCharacter.h"

AALSAEQABossCharacter::AALSAEQABossCharacter()
{
    BossComponent = CreateDefaultSubobject<UALSAEQABossComponent>(TEXT("BossComponent"));
    BossMaxHealth = 1200.0f;
    BossHealth = BossMaxHealth;
}

void AALSAEQABossCharacter::BeginPlay()
{
    Super::BeginPlay();
    BossHealth = FMath::Max(BossMaxHealth, 1.0f);
    if (BossComponent)
    {
        BossComponent->SetHealthPercent(1.0f);
        BossComponent->OnPhaseChanged.AddDynamic(this, &AALSAEQABossCharacter::HandleBossPhaseChanged);
    }
}

float AALSAEQABossCharacter::ReceiveALSAEQADamage_Implementation(const FALSAEQADamageInfo& DamageInfo)
{
    return ReceiveBossDamage(DamageInfo) ? DamageInfo.Amount : 0.0f;
}

bool AALSAEQABossCharacter::ReceiveBossDamage(const FALSAEQADamageInfo& DamageInfo)
{
    if (!BossComponent || BossComponent->IsDefeated() || DamageInfo.Amount <= 0.0f)
    {
        return false;
    }

    BossHealth = FMath::Max(0.0f, BossHealth - DamageInfo.Amount);
    const float Percent = BossHealth / FMath::Max(BossMaxHealth, 1.0f);
    BossComponent->SetHealthPercent(Percent);
    if (BossComponent->IsDefeated())
    {
        SetEnemyState(EALSAEQAEnemyState::Dead);
    }
    else if (DamageInfo.Instigator)
    {
        SetTargetActor(DamageInfo.Instigator);
        SetEnemyState(EALSAEQAEnemyState::Alert);
    }
    return true;
}

void AALSAEQABossCharacter::HandleBossPhaseChanged(EALSAEQABossPhase NewPhase)
{
    if (NewPhase == EALSAEQABossPhase::Defeated)
    {
        SetEnemyState(EALSAEQAEnemyState::Dead);
        return;
    }
    SetEnemyState(EALSAEQAEnemyState::Alert);
}
