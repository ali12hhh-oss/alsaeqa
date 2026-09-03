#include "Systems/ALSAEQAHealthComponent.h"

UALSAEQAHealthComponent::UALSAEQAHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAHealthComponent::ApplyDamage(float Damage)
{
    if (bDead || Damage <= 0.0f)
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health, MaxHealth);

    if (Health <= 0.0f)
    {
        bDead = true;
        OnDeath.Broadcast();
    }
}

void UALSAEQAHealthComponent::Heal(float Amount)
{
    if (bDead || Amount <= 0.0f)
    {
        return;
    }

    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health, MaxHealth);
}

void UALSAEQAHealthComponent::ResetHealth()
{
    bDead = false;
    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health, MaxHealth);
}

float UALSAEQAHealthComponent::GetHealthPercent() const
{
    return MaxHealth > KINDA_SMALL_NUMBER ? Health / MaxHealth : 0.0f;
}
