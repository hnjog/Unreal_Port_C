#include "CGuardPoint.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include<Components/CapsuleComponent.h>

ACGuardPoint::ACGuardPoint()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule",Scene);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(44.0f);
	Capsule->AddRelativeLocation(FVector(0.0f, 70.0f, 90.0f));
}

void ACGuardPoint::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
	
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACGuardPoint::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACGuardPoint::OnComponentEndOverlap);

	OffCollision();
	
	Super::BeginPlay();
}

void ACGuardPoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);

	// 현재 delegate가 ACharacter만 존재함
	// 따라서 delegate와 함수를 수정해야 할 필요성이 존재
	// 애초에 Attachment를 받고, 이 녀석의 owner 를 통해 가져오는 것이 맞다고 봄

	/*
		방어하는 용도의 Collision 임

		전달해야하는 것은
		나 자신과,
		외부 요인 정도면 되지 않을까?
	*/

	//CLog::Print(OtherActor->GetName());

	if (OnGuardPointBeginOverlap.IsBound() == true)
		OnGuardPointBeginOverlap.Broadcast(this,OtherActor);
}

void ACGuardPoint::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnGuardPointEndOverlap.IsBound() == true)
		OnGuardPointEndOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACGuardPoint::OnCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACGuardPoint::OffCollision()
{
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}