// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"




void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UTankAimingComponent* ControlledTankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ensure(PlayerPawn && ControlledTankAimingComponent)) { return; }


	// move tank towards player
	MoveToActor(PlayerPawn, AcceptanceRadius);

	// aim towards the player
	ControlledTankAimingComponent->AimAt(PlayerPawn->GetActorLocation());

	// Fire when locked
	if (ControlledTankAimingComponent->GetFiringState() == EFiringState::Locked)
	{
		ControlledTankAimingComponent->Fire();
	}
	
	
}
