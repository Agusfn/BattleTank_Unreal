// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"


void ATankPlayerController::BeginPlay() // no necesita "override"
{
	Super::BeginPlay(); // llamar BeginPlay de la/s clases superiores

	// Ubicamos el aiming component para pasarle la ref. por BP al UI Widget
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return;  }
	
	FoundAimingComponent(AimingComponent);
	
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTankTowardsCrosshair();
}


void ATankPlayerController::AimTankTowardsCrosshair()
{
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector OutHitLocation; // Out Parameter

	if (GetSightRayHitLocation(OutHitLocation)) // aiming at existing world point
	{
		AimingComponent->AimAt(OutHitLocation);
	}
		
}
	

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// obtenemos posicion del crosshair en la pantalla (en 2d)
	int32 OutViewPortSizeX, OutViewPortSizeY;
	GetViewportSize(OutViewPortSizeX, OutViewPortSizeY);

	FVector2D Crosshair2DLoc = FVector2D(OutViewPortSizeX * CrosshairXLocation, OutViewPortSizeY * CrosshairYLocation);

	// "des proyectar" la posición del crosshair en una dirección del mundo
	FVector OutLookDirection;
	if (GetLookDirectionFromScreenPoint(Crosshair2DLoc, OutLookDirection))
	{
		// Hacemos line tracing hacia esa direccion del mundo
		GetLookVectorHitLocation(OutLookDirection, OutHitLocation);
	}

	return true;
}


bool ATankPlayerController::GetLookDirectionFromScreenPoint(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector OutCameraWorldLocation; // not used

	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		OutCameraWorldLocation,
		LookDirection
	);
}


/* obtener ubicación del primer objeto que se interpone entre una recta que sale desde cámara en dirección LookDirection */
bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;

	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	else
	{
		HitLocation = FVector(0);
		return false;
	}

}