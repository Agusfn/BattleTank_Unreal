// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay() // no necesita "override"
{
	Super::BeginPlay(); // llamar BeginPlay de la/s clases superiores

	// Ubicamos el aiming component para pasarle la ref. por BP al UI Widget
	UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return;  }
	
	FoundAimingComponent(AimingComponent);
	
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		PossesedTank = Cast<ATank>(InPawn);

		if (!ensure(PossesedTank)) { return; }
		PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossesedTankDeath);
	}
}


void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTankTowardsCrosshair();
}


void ATankPlayerController::OnPossesedTankDeath()
{
	if (!PossesedTank) return;
	StartSpectatingOnly();
}



void ATankPlayerController::AimTankTowardsCrosshair()
{
	if (!GetPawn()) { return; }

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
		return GetLookVectorHitLocation(OutLookDirection, OutHitLocation);
	}

	return false;
}


// Obtiene la dirección en donde apunta el crosshair, saliendo de la cámara.
// Devuelve false si no se apunta a ningun objeto (creo)
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


// obtener la posicion del mundo de la interseccion entre una recta que sale desde cámara en dirección LookDirection, y el primer cuerpo que se interponga
// Devuelve true si existe la posicion, y false si no existe (se apunta al cielo)
bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;

	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + (LookDirection * LineTraceRange);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetPawn());

	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, CollisionParams))
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