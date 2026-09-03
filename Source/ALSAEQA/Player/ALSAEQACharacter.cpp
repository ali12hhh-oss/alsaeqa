#include "Player/ALSAEQACharacter.h"
#include "Systems/ALSAEQAAbilityComponent.h"
#include "Systems/ALSAEQAHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

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

void AALSAEQACharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AALSAEQACharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AALSAEQACharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AALSAEQACharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AALSAEQACharacter::PerformLightAttack()
{
    // Animation, hit detection and damage execution are intentionally data-driven
    // and will be connected to the combat system without hard-coding assets here.
}

void AALSAEQACharacter::PerformHeavyAttack()
{
    // Heavy attack hook for the combat state machine.
}

bool AALSAEQACharacter::ActivateAbility(EALSAEQAAbility Ability)
{
    return AbilityComponent && AbilityComponent->TryActivateAbility(Ability);
}
