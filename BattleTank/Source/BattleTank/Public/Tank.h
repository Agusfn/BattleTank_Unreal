// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercentage() const;

	FTankDelegate OnDeath;

private:

	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	// overrideada de APawn. Se llama automáticamente cuando se aplica daño con la funcion del juego UGameplayStatics::ApplyRadialDamage u otra
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 MaxHealth = 100;

	int32 CurrentHealth = 1;

};
