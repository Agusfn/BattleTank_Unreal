// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

UTankBarrel::UTankBarrel()
{

}

// -1 is max downward speed, 1 is max upward speed.
void UTankBarrel::Elevate(float RelativeSpeed)
{
	// Limitamos rapidez relativa a -1 y 1
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);

	// obtenemos la diferencia de grados de inclinaci�n a aplicar en este tick (frame)
	float ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	
	// Obtenemos la nueva elevaci�n a establecer
	float RawNewElevation = RelativeRotation.Pitch + ElevationChange;

	// Limitamos la elevaci�n final para que no exceda los l�mites
	RawNewElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(RawNewElevation, 0, 0));


}