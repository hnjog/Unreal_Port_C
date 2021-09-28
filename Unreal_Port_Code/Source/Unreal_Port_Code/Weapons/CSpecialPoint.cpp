#include "CSpecialPoint.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include<Components/CapsuleComponent.h>

ACSpecialPoint::ACSpecialPoint()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule",Scene);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(44.0f);
	Capsule->AddRelativeLocation(FVector(0.0f, 70.0f, 90.0f));
}

void ACSpecialPoint::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
	
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACSpecialPoint::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACSpecialPoint::OnComponentEndOverlap);

	OffCollision();
	
	Super::BeginPlay();
}

void ACSpecialPoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);

	if (OnGuardPointBeginOverlap.IsBound() == true)
		OnGuardPointBeginOverlap.Broadcast(this,OtherActor);

	if (OnSpecialBeginOverlap.IsBound() == true)
		OnSpecialBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACSpecialPoint::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnGuardPointEndOverlap.IsBound() == true)
		OnGuardPointEndOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));

	if (OnSpecialEndOverlap.IsBound() == true)
		OnSpecialEndOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}


void ACSpecialPoint::OnCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACSpecialPoint::OffCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACSpecialPoint::SetCapsuleSize(float height, float radius)
{
	Capsule->SetCapsuleHalfHeight(height);
	Capsule->SetCapsuleRadius(radius);
}

void ACSpecialPoint::AddCapsuleLocation(const FVector& relative)
{
	Capsule->AddRelativeLocation(relative);
}

void ACSpecialPoint::SetGenHit()
{
	Capsule->SetNotifyRigidBodyCollision(true);
}
