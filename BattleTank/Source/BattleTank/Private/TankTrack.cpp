// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpringSpawner.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Obtenemos lista de todas las SprungWheel contenidas en cada Spawner de este tank track.
TArray<class ASprungWheel*> UTankTrack::GetWheels() const
{
	OUT TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	TArray<ASprungWheel*> SprungWheels;

	for (USceneComponent* Child : Children)
	{
		USpringSpawner* Spawner = Cast<USpringSpawner>(Child);
		if (!Spawner) { continue; }

		AActor* Actor = Spawner->GetSpawnedWheel();
		ASprungWheel* SprungWheel = Cast<ASprungWheel>(Actor);
		if (!SprungWheel) continue;

		SprungWheels.Add(SprungWheel);
	}

	return SprungWheels;
}


void UTankTrack::SetThrottle(float ThrottleScale)
{
	float CurrentThrottle = FMath::Clamp<float>(ThrottleScale, -1, 1);
	DriveTrack(CurrentThrottle);
}



void UTankTrack::DriveTrack(float Throttle)
{
	float TotalForceMagnitude = Throttle * TrackMaxDrivingForce;

	auto Wheels = GetWheels();
	float ForceMagnitudePerWheel = TotalForceMagnitude / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForceMagnitudePerWheel);
	}

}
