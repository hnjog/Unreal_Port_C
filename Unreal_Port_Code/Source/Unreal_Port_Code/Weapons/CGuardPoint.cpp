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

	// ���� delegate�� ACharacter�� ������
	// ���� delegate�� �Լ��� �����ؾ� �� �ʿ伺�� ����
	// ���ʿ� Attachment�� �ް�, �� �༮�� owner �� ���� �������� ���� �´ٰ� ��

	/*
		����ϴ� �뵵�� Collision ��

		�����ؾ��ϴ� ����
		�� �ڽŰ�,
		�ܺ� ���� ������ ���� ������?
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