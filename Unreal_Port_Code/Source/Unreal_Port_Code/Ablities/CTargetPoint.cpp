#include "CTargetPoint.h"
#include "Global.h"
#include "Components/WidgetComponent.h"
#include "Components/CStateComponent.h"
#include"GameFramework/Character.h"
#include<Components/SphereComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACTargetPoint::ACTargetPoint()
{
	//PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &SceneComponent, "SceneComponent", GetRootComponent());
	CHelpers::CreateComponent(this, &Sphere, "Sphere", SceneComponent);
	CHelpers::CreateComponent(this, &TargetWidget, "TargetWidget", Sphere);

	TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetWidget->SetVisibility(false);

	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void ACTargetPoint::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CharacterMovement = CHelpers::GetComponent<UCharacterMovementComponent>(OwnerCharacter);
	Mesh = CHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);

	if (!!OwnerCharacter->GetController())
	{
		TargetWidget->InitWidget();
		TargetWidget->SetRelativeLocation(FVector(0, 0, 80));
		TargetWidget->SetDrawSize(FVector2D(30, 30));
	}

}

//void ACTargetPoint::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

void ACTargetPoint::TargetWidgetVisible(bool val)
{
	TargetWidget->SetVisibility(val);
}

bool ACTargetPoint::IsTargetStiffed()
{
	return State->IsStiffMode();
}

bool ACTargetPoint::IsTargetKnockOuted()
{
	return State->IsKnockOutMode();
}

bool ACTargetPoint::IsTargetFalling()
{
	return CharacterMovement->IsFalling();
}

FVector ACTargetPoint::TargetLocation()
{
	return GetActorLocation();
}

// 와이어를 당길 때 - 러시 시작 때 호출
void ACTargetPoint::TargetBeaten()
{
	State->SetStiffMode();
	CharacterMovement->Velocity = FVector::ZeroVector;
	CharacterMovement->GravityScale = 0.0f;
	Mesh->SetEnableGravity(false);
}

// AirCombo 끝날 떄 호출될 예정
void ACTargetPoint::End_TargetBeaten()
{
	//State->SetIdleMode();
	CharacterMovement->GravityScale = 1.0f;
	Mesh->SetEnableGravity(true);
}