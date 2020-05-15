// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{

	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;

}


void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }

	//UE_LOG(LogTemp, Warning, TEXT("%s. Forward throw: %f"), *GetOwner()->GetName(), Throw)

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

}


void UTankMovementComponent::IntendTurnRight(float Throw)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s. Right throw: %f"), *GetOwner()->GetName(), Throw)
	if (!ensure(LeftTrack && RightTrack)) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);

}

// Usado para movimiento AI
void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s being requested direct move."), *GetOwner()->GetName())

	// Obtenemos vectores unitarios de la dirección donde apunta el tanque, y la dirección donde debe dirigirse.
	FVector TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AITargetDirection = MoveVelocity.GetSafeNormal();

	/* Si tenemos el coseno del ángulo entre estos dos vectores (el de la dirección donde apunta el tanque y la dirección a donde debe dirigirse, todo en ese instante de tiempo)
	 vamos a tener un valor entre -1 y 1. Será 1 si el ángulo es 0º (el tanque apunta a donde debe ir), será 0 si el ángulo es 90º (el tanque apunta perpendicular a donde debe ir)
	 y será -1 si el ángulo es 180º (el tanque apunta en dirección opuesta a donde debe ir).

	 Con este coseno determinaremos en qué sentido y qué tan fuerte acelerará el tanque según la dirección donde debe ir, en cada cuadro.

	 Como ambos vectores son unitarios (miden 1), por propiedad matemática, el producto punto entre ellos es igual al coseno de su ángulo.
	 */

	float ForwardThrow = FVector::DotProduct(TankForwardDirection, AITargetDirection);

	IntendMoveForward(ForwardThrow);



	/*
	TODO crear comentario explicando funcionamiento del coseno del ángulo y el producto vectorial.
	*/

	float RightThrow = FVector::CrossProduct(TankForwardDirection, AITargetDirection).Z;
	IntendTurnRight(RightThrow);



}
