// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Firing status enum
UENUM()
enum class EFiringState : uint8
{
	Locked,
	Aiming,
	Reloading,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetProjectileBP(TSubclassOf<AProjectile> ProjectileBP);


	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	void AimAt(FVector Location);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoAmount() const;

	EFiringState GetFiringState() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint; // alguna clase subclase de AProjectile. En este caso va a ser únicamente Projectile_BP

private:	

	// Sets default values for this component's properties
	UTankAimingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveBarrelTowardsDirection(FVector AimDirection);

	bool IsBarrelMoving();


	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	// Permite editar el parámetro en la interfaz de unreal. Para la clase o cualquier instancia individualmente.
	UPROPERTY(EditAnywhere, Category = "Firing")
	float ProjectileSpeed = 4000; // 40m/s

	// Permite sólo editar la propiedad para la clase de forma general, no individualmente para cada instancia.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 AmmoAmount = 10;

	double LastFireTime = 0;

	FVector TargetAimDirection;



		
};
