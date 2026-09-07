#include "Systems/ALSAEQAHealthComponent.h"
#include "Systems/ALSAEQAInjuryComponent.h"
#include "GameFramework/Actor.h"

UALSAEQAHealthComponent::UALSAEQAHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UALSAEQAHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner) return;

    InjuryComponent = Owner->FindComponentByClass<UALSAEQAInjuryComponent>();
    if (!InjuryComponent)
    {
        InjuryComponent = NewObject<UALSAEQAInjuryComponent>(Owner, TEXT("UniversalInjuryComponent"));
        if (InjuryComponent)
        {
            InjuryComponent->RegisterComponent();
        }
    }

    if (InjuryComponent)
    {
        InjuryComponent->OnFatalState.AddDynamic(this, &UALSAEQAHealthComponent::HandleInjuryDeath);
    }
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
        if (InjuryComponent) InjuryComponent->MarkDead();
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
    if (InjuryComponent) InjuryComponent->ResetAfterDeath();
    OnHealthChanged.Broadcast(Health, MaxHealth);
}

void UALSAEQAHealthComponent::HandleInjuryDeath()
{
    if (bDead) return;
    bDead = true;
    Health = 0.0f;
    OnHealthChanged.Broadcast(Health, MaxHealth);
    OnDeath.Broadcast();
}

float UALSAEQAHealthComponent::GetHealthPercent() const
{
    return MaxHealth > KINDA_SMALL_NUMBER ? Health / MaxHealth : 0.0f;
}
