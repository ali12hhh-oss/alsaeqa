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
#include "Cinematic/ALSAEQACinematicDirector.h"
#include "Progression/ALSAEQAStageFlowComponent.h"
#include "Progression/ALSAEQAStageObjectiveComponent.h"
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
    CinematicDirector = CreateDefaultSubobject<UALSAEQACinematicDirector>(TEXT("CinematicDirector"));
    StageFlowComponent = CreateDefaultSubobject<UALSAEQAStageFlowComponent>(TEXT("StageFlowComponent"));
    StageObjectiveComponent = CreateDefaultSubobject<UALSAEQAStageObjectiveComponent>(TEXT("StageObjectiveComponent"));
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
