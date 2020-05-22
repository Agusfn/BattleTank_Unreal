// Fill out your copyright notice in the Description page of Project Settings.


#include "SpringSpawner.h"
#include "Engine/World.h" 
#include "Kismet/GameplayStatics.h" 

// Sets default values for this component's properties
USpringSpawner::USpringSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USpringSpawner::BeginPlay()
{
	Super::BeginPlay();

	Wheel = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform());
	if (!Wheel) return;


	Wheel->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	UGameplayStatics::FinishSpawningActor(Wheel, GetComponentTransform());
	
}


// Called every frame
void USpringSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* USpringSpawner::GetSpawnedWheel() const
{ 
	return Wheel; 
}