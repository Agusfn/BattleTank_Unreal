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

	// Obtenemos vectores unitarios de la direcci�n donde apunta el tanque, y la direcci�n donde debe dirigirse.
	FVector TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AITargetDirection = MoveVelocity.GetSafeNormal();

	/* Si tenemos el coseno del �ngulo entre estos dos vectores (el de la direcci�n donde apunta el tanque y la direcci�n a donde debe dirigirse, todo en ese instante de tiempo)
	 vamos a tener un valor entre -1 y 1. Ser� 1 si el �ngulo es 0� (el tanque apunta a donde debe ir), ser� 0 si el �ngulo es 90� (el tanque apunta perpendicular a donde debe ir)
	 y ser� -1 si el �ngulo es 180� (el tanque apunta en direcci�n opuesta a donde debe ir).

	 Con este coseno determinaremos en qu� sentido y qu� tan fuerte acelerar� el tanque seg�n la direcci�n donde debe ir, en cada cuadro.

	 Como ambos vectores son unitarios (miden 1), por propiedad matem�tica, el producto punto entre ellos es igual al coseno de su �ngulo.
	 */

	float ForwardThrow = FVector::DotProduct(TankForwardDirection, AITargetDirection);

	IntendMoveForward(ForwardThrow);



	/*
	TODO crear comentario explicando funcionamiento del coseno del �ngulo y el producto vectorial.
	*/

	float RightThrow = FVector::CrossProduct(TankForwardDirection, AITargetDirection).Z;
	IntendTurnRight(RightThrow);



}
