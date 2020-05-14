// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

// Forward declarations
class UTankBarrel;
class UTankTurret;
class UTankAimingComponent;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable)
	void Fire();


private:

	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	// Permite editar el parámetro en la interfaz de unreal. Para la clase o cualquier instancia individualmente.
	UPROPERTY(EditAnywhere, Category = Firing)
	float ProjectileSpeed = 8000; // 80m/s
	
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint; // alguna clase subclase de AProjectile. En este caso va a ser únicamente Projectile_BP

	UTankBarrel* TankBarrel = nullptr;

	// Permite sólo editar la propiedad para la clase de forma general, no individualmente para cada instancia.
	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

protected:
	
	// Para poder ser accedido desde BP
	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent* AimingComponent = nullptr;

	// "BlueprintReadOnly" para poder acceder a este componente desde el blueprint del tanque.
	// En este caso no usamos esto de abajo, porque no lo vamos a usar como componente fijo heredado, sino que se agrega desde el blueprint editor, pero queda como ejemplo.
	//UPROPERTY(BlueprintReadOnly)
	//UTankMovementComponent* MovementComponent = nullptr;


};
