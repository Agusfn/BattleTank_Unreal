// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


}


void ATank::BeginPlay()
{
	Super::BeginPlay();

	AimingComponent = FindComponentByClass<UTankAimingComponent>();
}


void ATank::AimAt(FVector TargetLocation)
{
	if (!ensure(AimingComponent)) { return;  }
	if (!AimingComponent) { return; }

	AimingComponent->AimAt(TargetLocation, ProjectileSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at location: %s"), *GetName(), *TargetLocation.ToString())

	UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at location: %s"), *GetName(), *TargetLocation.ToString())
}


void ATank::Fire()
{
	if (!ensure(TankBarrel)) { return; }

	bool bIsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (bIsReloaded)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>( // projectile base class
			ProjectileBlueprint, // projectile blueprint class
			TankBarrel->GetSocketLocation(FName("Muzzle")),
			TankBarrel->GetSocketRotation(FName("Muzzle"))
			);

		Projectile->LaunchProjectile(ProjectileSpeed);

		LastFireTime = FPlatformTime::Seconds(); // GetWorld()->GetTimeSeconds() es buena opcion tambien
	}

}
