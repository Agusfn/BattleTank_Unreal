// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}


// overrideado, se llama cuando se toma posesion del pawn
void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		PossesedTank = Cast<ATank>(InPawn);

		if (!PossesedTank) { return; } // may be a mortar

		//suscribir metodo local a evento de muerte tanque
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossesedTankDeath);
	}
}

void ATankAIController::OnPossesedTankDeath()
{
	if (!PossesedTank) return;
	PossesedTank->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UTankAimingComponent* ControlledTankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ensure(PlayerPawn && ControlledTankAimingComponent)) { return; }


	// move tank towards player (mortar won't have effect)
	MoveToActor(PlayerPawn, AcceptanceRadius);

	// aim towards the player
	ControlledTankAimingComponent->AimAt(PlayerPawn->GetActorLocation());

	// Fire when locked
	if (ControlledTankAimingComponent->GetFiringState() == EFiringState::Locked)
	{
		ControlledTankAimingComponent->Fire();
	}
	
	
}
