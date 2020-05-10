// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

//
void UTankTrack::SetThrottle(float ThrottleScale)
{
	// GetForwardVector da la dirección en la que apunta este componente
	FVector PushForce = GetForwardVector() * ThrottleScale * TrackMaxDrivingForce;

	// Ubicación del centro del track
	FVector ForceLocation = GetComponentLocation();

	// GetOwner devuelve el Tank_BP, GetRootComponent devuelve el Tank mesh.
	// Casteamos a UPrimitiveComponent ya que es la clase de nivel más bajo que permite aplicar fuerza
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	// Aplicamos fuerza al mesh Tank.
	TankRoot->AddForceAtLocation(PushForce, ForceLocation);

	//UE_LOG(LogTemp, Warning, TEXT("Applying force of %f to track %s. Input scale: %f"), PushForce.Size(), *GetName(), ThrottleScale)
}