#include "Creatures/ALSAEQAGiantSnakeAIController.h"

#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Creatures/ALSAEQAGiantSnake.h"

AALSAEQAGiantSnakeAIController::AALSAEQAGiantSnakeAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AALSAEQAGiantSnakeAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    SetSnakeState(EALSAEQAGiantSnakeState::Hidden);
}

void AALSAEQAGiantSnakeAIController::AcquireTarget(AActor* NewTarget)
{
    TargetActor = NewTarget;
    if (TargetActor)
    {
        SetSnakeState(EALSAEQAGiantSnakeState::Chase);
    }
}

void AALSAEQAGiantSnakeAIController::SetSnakeState(EALSAEQAGiantSnakeState NewState)
{
    State = NewState;

    AALSAEQAGiantSnake* Snake = Cast<AALSAEQAGiantSnake>(GetPawn());
    if (!Snake)
    {
        return;
    }

    switch (State)
    {
    case EALSAEQAGiantSnakeState::Ambush:
        Snake->StartAmbush();
        break;
    case EALSAEQAGiantSnakeState::Coil:
        Snake->CoilAroundObstacle();
        break;
    case EALSAEQAGiantSnakeState::Retreat:
        Snake->Retreat();
        break;
    default:
        break;
    }
}

void AALSAEQAGiantSnakeAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AALSAEQAGiantSnake* Snake = Cast<AALSAEQAGiantSnake>(GetPawn());
    if (!Snake)
    {
        return;
    }

    if (Snake->IsDefeated())
    {
        SetSnakeState(EALSAEQAGiantSnakeState::Defeated);
        return;
    }

    if (!TargetActor)
    {
        TargetActor = UGameplayStatics::GetPlayerPawn(this, 0);
    }

    if (!TargetActor)
    {
        return;
    }

    const float Distance = FVector::Dist(Snake->GetActorLocation(), TargetActor->GetActorLocation());

    if (Distance <= AttackRange)
    {
        SetSnakeState(EALSAEQAGiantSnakeState::Attack);
    }
    else if (Distance <= DetectionRange)
    {
        SetSnakeState(EALSAEQAGiantSnakeState::Chase);
    }
    else
    {
        SetSnakeState(EALSAEQAGiantSnakeState::Hidden);
    }
}
