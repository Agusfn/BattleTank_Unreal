// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"


UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

// se llama en cada cuadro donde las tracks están en contacto con la tierra.
void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;
}



void UTankTrack::ApplySidewaysForce()
{
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	FVector CorrectionForce = TankRoot->GetMass() * CorrectionAcceleration / 2; // two tracks

	TankRoot->AddForce(CorrectionForce);
}


//
void UTankTrack::SetThrottle(float ThrottleScale)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + ThrottleScale, -1, 1);
}

void UTankTrack::DriveTrack()
{
	// GetForwardVector da la dirección en la que apunta este componente
	FVector PushForce = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;

	// Ubicación del centro del track
	FVector ForceLocation = GetComponentLocation();

	// GetOwner devuelve el Tank_BP, GetRootComponent devuelve el Tank mesh.
	// Casteamos a UPrimitiveComponent ya que es la clase de nivel más bajo que permite aplicar fuerza
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	// Aplicamos fuerza al mesh Tank.
	TankRoot->AddForceAtLocation(PushForce, ForceLocation);

	//UE_LOG(LogTemp, Warning, TEXT("Applying force of %f to track %s. Input scale: %f"), PushForce.Size(), *GetName(), ThrottleScale)
}
