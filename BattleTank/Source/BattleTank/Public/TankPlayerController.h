// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * Responsible for helping the player aim
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);

private:

	// override para asegurarse que exista este método en alguna de las clases padre, y reemplazarlo 
	// (TankPlayerController <- PlayerController <- Actor(ESTA))
	// (el método en la clase superior tiene "virtual" en su declaracion para permitir esto, y este hace lo mismo para permitir una clase inferior overridear)
	virtual void BeginPlay() override; 
	
	virtual void Tick(float DeltaTime) override;

	void AimTankTowardsCrosshair();

	bool GetSightRayHitLocation(FVector &OutHitLocation) const;

	bool GetLookDirectionFromScreenPoint(FVector2D ScreenLocation, FVector& WorldDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5; // ubicación relativa del ancho de la pantalla en donde se encuentra el crosshair

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.3333; // idem, del alto, comenzando desde arriba.

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000; // unreal units (cm) (10km)

};
