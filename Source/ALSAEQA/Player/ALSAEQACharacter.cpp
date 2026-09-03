#include "Player/ALSAEQACharacter.h"
#include "Systems/ALSAEQAHealthComponent.h"
#include "Storm/ALSAEQAThunderChargeComponent.h"
#include "Storm/ALSAEQAThunderEnvironmentComponent.h"
#include "Storm/ALSAEQADynamicStormSubsystem.h"
#include "Story/ALSAEQALegacyComponent.h"
#include "Companions/ALSAEQARidingComponent.h"
#include "Companions/ALSAEQAMountActor.h"
#include "Companions/ALSAEQAMountComponent.h"
#include "Save/ALSAEQASaveManager.h"
#include "Visual/ALSAEQAVisualAssetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Combat/ALSAEQADamageTypes.h"
#include "Combat/ALSAEQADamageReceiver.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "EngineUtils.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "GameFramework/PlayerController.h"

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
    MeleeCombatComponent = CreateDefaultSubobject<UALSAEQAMeleeCombatComponent>(TEXT("MeleeCombatComponent"));
    HealthComponent = CreateDefaultSubobject<UALSAEQAHealthComponent>(TEXT("HealthComponent"));
    ThunderChargeComponent = CreateDefaultSubobject<UALSAEQAThunderChargeComponent>(TEXT("ThunderChargeComponent"));
    LegacyComponent = CreateDefaultSubobject<UALSAEQALegacyComponent>(TEXT("LegacyComponent"));
    RidingComponent = CreateDefaultSubobject<UALSAEQARidingComponent>(TEXT("RidingComponent"));
    VisualAssetComponent = CreateDefaultSubobject<UALSAEQAVisualAssetComponent>(TEXT("VisualAssetComponent"));
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
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &AALSAEQACharacter::HandlePlayerDeath);
    }
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
    PlayerInputComponent->BindAction(TEXT("Mount"), IE_Pressed, this, &AALSAEQACharacter::MountOrDismount);
    PlayerInputComponent->BindAction(TEXT("ThunderCharge"), IE_Pressed, this, &AALSAEQACharacter::BeginThunderCharge);
    PlayerInputComponent->BindAction(TEXT("ThunderCharge"), IE_Released, this, &AALSAEQACharacter::ReleaseThunderCharge);
    PlayerInputComponent->BindAction(TEXT("MountLightningDash"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountLightningDash);
    PlayerInputComponent->BindAction(TEXT("MountThunderRoar"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountThunderRoar);
    PlayerInputComponent->BindAction(TEXT("MountLightningKick"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountLightningKick);
    PlayerInputComponent->BindAction(TEXT("MountStormCharge"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountStormCharge);
    PlayerInputComponent->BindAction(TEXT("MountLightningCrossing"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountLightningCrossing);
    PlayerInputComponent->BindAction(TEXT("MountStormLeap"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountStormLeap);
    PlayerInputComponent->BindAction(TEXT("MountLightningShield"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountLightningShield);
    PlayerInputComponent->BindAction(TEXT("MountStormSummon"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountStormSummon);
    PlayerInputComponent->BindAction(TEXT("MountThunderSense"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountThunderSense);
    PlayerInputComponent->BindAction(TEXT("MountStormMode"), IE_Pressed, this, &AALSAEQACharacter::ActivateMountStormMode);
}

void AALSAEQACharacter::MoveForward(float Value)
{
    if (bDeathInProgress) return;
    if (RidingComponent && RidingComponent->IsRiding()) { RidingComponent->MoveForward(Value); return; }
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator R(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(R).GetUnitAxis(EAxis::X), Value);
    }
}

void AALSAEQACharacter::MoveRight(float Value)
{
    if (bDeathInProgress) return;
    if (RidingComponent && RidingComponent->IsRiding()) { RidingComponent->MoveRight(Value); return; }
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator R(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(R).GetUnitAxis(EAxis::Y), Value);
    }
}

