// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h" 
#include "Engine.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds(); // GetWorld()->GetTimeSeconds() es buena opcion tambien
}


void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (FiringState != EFiringState::OutOfAmmo)
	{
		if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
		{
			FiringState = EFiringState::Reloading;
		}
		else if (IsBarrelMoving())
		{
			FiringState = EFiringState::Aiming;
		}
		else
		{
			FiringState = EFiringState::Locked;
		}
	}

}


void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::SetProjectileBP(TSubclassOf<AProjectile> ProjectileBP)
{
	ProjectileBlueprint = ProjectileBP;
}


int32 UTankAimingComponent::GetAmmoAmount() const
{
	return AmmoAmount;
}

void UTankAimingComponent::AimAt(FVector TargetLocation)
{
	if (!ensure(Barrel)) {	return; }

	FVector ProjectileExitLocation = Barrel->GetSocketLocation(FName("Muzzle"));
	FVector OutNeededProjectileVelocity;

	bool bLaunchPossible = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutNeededProjectileVelocity,
		ProjectileExitLocation,
		TargetLocation,
		ProjectileSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bLaunchPossible) 
	{
		// Obtenemos dirección de velocidad necesaria (vector unitario)
		FVector AimDirection = OutNeededProjectileVelocity.GetSafeNormal();
		MoveBarrelTowardsDirection(AimDirection);
	}
	
}


void UTankAimingComponent::MoveBarrelTowardsDirection(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	TargetAimDirection = AimDirection;

	// obtener diferencia entre la rotacion actual del barrel, y la AimDirection
	FRotator CurrentBarrelRotation = Barrel->GetForwardVector().Rotation(); // dirección absoluta del cañon en coordenadas de rotación (pitch, yaw)
	FRotator AimDirectionRotator = AimDirection.Rotation(); // dirección necesaria absoluta (en coordenadas de rotacion) en dirección al objetivo
	
	FRotator DeltaRotator = AimDirectionRotator - CurrentBarrelRotation;

	// mover una cantidad determinada el barrel este frame
	Barrel->Elevate(DeltaRotator.Pitch);

	// Shortest way
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
	
}


void UTankAimingComponent::Fire()
{

	if (!ensure(Barrel && ProjectileBlueprint)) { return; }

	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>( // projectile base class
			ProjectileBlueprint, // projectile blueprint class
			Barrel->GetSocketLocation(FName("Muzzle")),
			Barrel->GetSocketRotation(FName("Muzzle"))
		);

		Projectile->LaunchProjectile(ProjectileSpeed);

		LastFireTime = FPlatformTime::Seconds();
		AmmoAmount -= 1;

		if (AmmoAmount == 0) {
			FiringState = EFiringState::OutOfAmmo;
		}
	}

}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}


bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false;  }

	FVector CurrentBarrelDirection = Barrel->GetForwardVector();


	if (CurrentBarrelDirection.Rotation().Equals(TargetAimDirection.Rotation(), 1)) 
	{
		return false;
	}
	else
	{
		return true;
	}
}