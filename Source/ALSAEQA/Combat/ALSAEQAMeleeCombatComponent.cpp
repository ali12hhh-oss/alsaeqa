#include "Combat/ALSAEQAMeleeCombatComponent.h"

#include "Systems/ALSAEQAHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UALSAEQAMeleeCombatComponent::UALSAEQAMeleeCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAMeleeCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UALSAEQAMeleeCombatComponent::CanAttack() const
{
    const UWorld* World = GetWorld();
    return World && (World->GetTimeSeconds() - LastAttackTime) >= AttackCooldown;
}

bool UALSAEQAMeleeCombatComponent::LightAttack()
{
    if (!CanAttack()) return false;
    LastAttackTime = GetWorld()->GetTimeSeconds();
    OnAttackStarted.Broadcast(false);
    return true;
}

bool UALSAEQAMeleeCombatComponent::HeavyAttack()
{
    if (!CanAttack()) return false;
    LastAttackTime = GetWorld()->GetTimeSeconds();
    OnAttackStarted.Broadcast(true);
    return true;
}

bool UALSAEQAMeleeCombatComponent::TryHitActor(AActor* Target, float Damage, EALSAEQADamageType DamageType)
{
    if (!IsValid(Target) || Target == GetOwner() || Damage <= 0.0f) return false;

    UALSAEQAHealthComponent* Health = Target->FindComponentByClass<UALSAEQAHealthComponent>();
    if (!Health || Health->IsDead()) return false;

    FALSAEQADamageInfo Info;
    Info.Amount = Damage;
    Info.Type = DamageType;
    Info.Instigator = GetOwner();
    Info.HitLocation = Target->GetActorLocation();
    Health->ApplyDamageInfo(Info);
    return true;
}
