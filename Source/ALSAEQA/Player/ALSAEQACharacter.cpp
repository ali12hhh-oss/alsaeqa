#include "Player/ALSAEQACharacter.h"
#include "Systems/ALSAEQAHealthComponent.h"
#include "Storm/ALSAEQAThunderChargeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Combat/ALSAEQADamageTypes.h"

AALSAEQACharacter::AALSAEQACharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 360.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 12.0f;
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    AbilityComponent = CreateDefaultSubobject<UALSAEQAAbilityComponent>(TEXT("AbilityComponent"));
    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    ThunderChargeComponent = CreateDefaultSubobject<UALSAEQAThunderChargeComponent>(TEXT("ThunderChargeComponent"));
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->BrakingDecelerationWalking = 1800.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

void AALSAEQACharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AALSAEQACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AALSAEQACharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AALSAEQACharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AALSAEQACharacter::LookUp);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AALSAEQACharacter::Turn);
    PlayerInputComponent->BindAction(TEXT("LightAttack"), IE_Pressed, this, &AALSAEQACharacter::PerformLightAttack);
    PlayerInputComponent->BindAction(TEXT("HeavyAttack"), IE_Pressed, this, &AALSAEQACharacter::PerformHeavyAttack);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AALSAEQACharacter::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AALSAEQACharacter::StopSprint);
    PlayerInputComponent->BindAction(TEXT("ThunderCharge"), IE_Pressed, this, &AALSAEQACharacter::BeginThunderCharge);
    PlayerInputComponent->BindAction(TEXT("ThunderCharge"), IE_Released, this, &AALSAEQACharacter::ReleaseThunderCharge);
}

void AALSAEQACharacter::MoveForward(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator ControlRotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
    }
}

void AALSAEQACharacter::MoveRight(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator ControlRotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
    }
}

void AALSAEQACharacter::LookUp(float Value) { AddControllerPitchInput(Value); }
void AALSAEQACharacter::Turn(float Value) { AddControllerYawInput(Value); }
void AALSAEQACharacter::StartSprint() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; }
void AALSAEQACharacter::StopSprint() { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; }
void AALSAEQACharacter::PerformLightAttack() {}
void AALSAEQACharacter::PerformHeavyAttack() {}

void AALSAEQACharacter::BeginThunderCharge()
{
    if (ThunderChargeComponent && AbilityComponent && AbilityComponent->HasAbility(EALSAEQAAbility::ThunderShock))
    {
        ThunderChargeComponent->BeginCharge();
    }
}

void AALSAEQACharacter::ReleaseThunderCharge()
{
    if (!ThunderChargeComponent || !AbilityComponent || !AbilityComponent->HasAbility(EALSAEQAAbility::ThunderShock))
    {
        return;
    }

    const float ChargePercent = ThunderChargeComponent->GetChargePercent();
    if (ChargePercent < 0.15f || AbilityComponent->GetEnergy() < ThunderShockEnergyCost)
    {
        ThunderChargeComponent->CancelCharge();
        return;
    }

    const float ReleasedPercent = ThunderChargeComponent->ReleaseCharge();
    if (ReleasedPercent <= 0.0f)
    {
        return;
    }

    AbilityComponent->ConsumeEnergy(ThunderShockEnergyCost);

    const FALSAEQADamageInfo DamageInfo{
        ThunderReleaseDamage * ThunderChargeComponent->GetReleasedDamageMultiplier(ReleasedPercent),
        EALSAEQADamageType::Thunder,
        this,
        GetActorLocation() + GetActorForwardVector() * 120.0f
    };
    LastThunderDamage = DamageInfo.Amount;
    LastThunderDamageInfo = DamageInfo;
}

void AALSAEQACharacter::CancelThunderCharge()
{
    if (ThunderChargeComponent) { ThunderChargeComponent->CancelCharge(); }
}

bool AALSAEQACharacter::ActivateAbility(EALSAEQAAbility Ability)
{
    return AbilityComponent && AbilityComponent->TryActivateAbility(Ability);
}
