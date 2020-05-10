// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"



void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATank* ControlledTank = Cast<ATank>(GetPawn());
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (ensure(PlayerTank)) {


		// move tank towards player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// aim towards the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		// fire when ready
		//ControlledTank->Fire();
	}
	
}
