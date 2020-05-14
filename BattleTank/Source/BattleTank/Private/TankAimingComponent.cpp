// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "Kismet/GameplayStatics.h" 
#include "Engine.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}



void UTankAimingComponent::AimAt(FVector TargetLocation)
{
	if (!ensure(Barrel)) {	return; }

	//UE_LOG(LogTemp, Warning, TEXT("AimingComponent: Tank %s aiming at location: %s"), *GetOwner()->GetName(), *TargetLocation.ToString())

	FVector ProjectileExitLocation = Barrel->GetSocketLocation(FName("Muzzle"));

	FVector NeededProjectileVelocity; // Out Param

	bool bLaunchPossible = UGameplayStatics::SuggestProjectileVelocity(
		this,
		NeededProjectileVelocity,
		ProjectileExitLocation,
		TargetLocation,
		ProjectileSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bLaunchPossible) {
		
		// Obtenemos dirección de velocidad necesaria (vector unitario)
		FVector AimDirection = NeededProjectileVelocity.GetSafeNormal();

		MoveBarrelTowardsDirection(AimDirection);

		/*FString TankName = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at: %s. Launch possible. Needed direction: %s."), *TankName, *TargetLocation.ToString(), *AimDirection.ToString());*/
	}
	else
	{
		// no aim solution
	}

}


void UTankAimingComponent::MoveBarrelTowardsDirection(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	// obtener diferencia entre la rotacion actual del barrel, y la AimDirection
	// mover una cantidad determinada el barrel este frame

	FRotator CurrentBarrelRotation = Barrel->GetForwardVector().Rotation(); // dirección absoluta del cañon en coordenadas de rotación (pitch, yaw)
	FRotator AimDirectionRotator = AimDirection.Rotation(); // dirección necesaria absoluta (en coordenadas de rotacion) en dirección al objetivo
	FRotator DeltaRotator = AimDirectionRotator - CurrentBarrelRotation;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);

	//UE_LOG(LogTemp, Warning, TEXT("Tank %s, current barrel rot: %s, target rot: %s"), *GetOwner()->GetName(), *CurrentBarrelRotation.ToString(), *AimDirectionRotator.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Tank %s, target yaw: %f, current turret yaw: %f, diff: %f"), *GetOwner()->GetName(), AimDirectionRotator.Yaw, CurrentBarrelRotation.Yaw, (AimDirectionRotator.Yaw - CurrentBarrelRotation.Yaw));
}

