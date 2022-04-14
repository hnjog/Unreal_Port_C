#include "CDodgePoint.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include"Components/CapsuleComponent.h"
#include"Components/CStateComponent.h"

ACDodgePoint::ACDodgePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Scene);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(44.0f);
	Capsule->AddRelativeLocation(FVector(0.0f, 70.0f, 90.0f));
}

void ACDodgePoint::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACDodgePoint::OnComponentBeginOverlap);

	IsSpawned = true;
	OutTimer = OutTime;
}

void ACDodgePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSpawned == true)
	{
		OutTimer -= DeltaTime;
		if (OutTimer < 0)
		{
			OnSuccessDodge.Unbind();
			Destroy();
		}
	}

}

void ACDodgePoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);

	CLog::Print("in dodge");

	if (OnSuccessDodge.IsBound() == true)
		OnSuccessDodge.Execute();

}

