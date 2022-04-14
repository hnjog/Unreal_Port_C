#include "CEnemy_Base.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include<Components/CapsuleComponent.h>
#include "Components/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CMontagesComponent.h"
#include<Weapons/CDamageType_LastCombo.h>
#include<Weapons/CDamageType_Counter.h>
#include<Weapons/CDamageType_AirCombo.h>
#include<Weapons/CDamageType_AirLast.h>
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Curves/CurveFloat.h"
#include<Materials/MaterialParameterCollection.h>
#include<Kismet/KismetMaterialLibrary.h>
#include<Widgets/CUserWidget_HealthBar.h>
#include<Ablities/CTargetPoint.h>

ACEnemy_Base::ACEnemy_Base()
	:bDamagedLastAttack(false), WakeUpTimer(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &HealthWidget, "HealthWidget", GetRootComponent());
	CHelpers::CreateComponent(this, &TargetPoint, "TargetPoint", GetMesh());

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");

	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Enemy/BaseAI/Curve_Special.Curve_Special'");

	CHelpers::GetClass<UCUserWidget_HealthBar>(&HealthWidgetClass, "WidgetBlueprint'/Game/Widzets/WBP_Health.WBP_Health_C'");
	HealthWidget->SetWidgetClass(HealthWidgetClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 80));
	HealthWidget->SetDrawSize(FVector2D(70, 30));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	CHelpers::GetClass<ACTargetPoint>(&TargetClass, "Blueprint'/Game/Player/BP_CTargetPoint.BP_CTargetPoint_C'");

}

void ACEnemy_Base::BeginPlay()
{
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_Base::OnStateTypeChanged);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemy_Base::OnComponentHit);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

	UMaterialInstanceConstant* body;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Enemy/SlimeNTurtles/Materials/MI_Slime.MI_Slime'");
	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	GetMesh()->SetMaterial(0, BodyMaterial);

	Super::BeginPlay();

	if (!!GetController())
	{
		HealthWidget->InitWidget();

		Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
		Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->SetVisible(false);
	}

	ACTargetPoint* targetPoint = GetWorld()->SpawnActorDeferred<ACTargetPoint>(TargetClass, TargetPoint->GetComponentTransform(), this);
	targetPoint->AttachToComponent(TargetPoint, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	targetPoint->SetActorRelativeLocation(FVector::ZeroVector);
	targetPoint->TargetWidgetVisible(false);
	UGameplayStatics::FinishSpawningActor(targetPoint, TargetPoint->GetComponentTransform());
	Targets.AddUnique(targetPoint);
}

void ACEnemy_Base::Tick(float DeltaTime)
{
	if (bUseLerpTurn == true)
	{
		LerpTurn(DeltaTime);
	}

	if (State->IsKnockOutMode() == true)
	{
		Popcorn();

		CheckTrue(GetCharacterMovement()->IsFalling());

		WakeUpTimer += DeltaTime;

		if (WakeUpTimer >= WakeUpTime && FMath::IsNearlyZero(GetVelocity().Size()) == true)
		{
			State->SetWakeUpMode();
			WakeUpTimer = 0.0f;
			return;
		}
	}
}

float ACEnemy_Base::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	Action->AbortByDamage();
	AboryByDamage();

	DamageUpdate(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	if (DamageEvent.DamageTypeClass == UCDamageType_AirLast::StaticClass())
	{
		ComboCount = 0;
		Action->CancelSpecial();
		if (State->IsKnockOutMode() == false)
			bDamagedLastAttack = true;
		State->SetKnockOutMode();
		LauchUpValue = 0.0f;
		return Status->GetHealth();
	}

	if (DamageEvent.DamageTypeClass == UCDamageType_AirCombo::StaticClass())
	{
		ComboCount++;
		State->SetStiffMode();
		return Status->GetHealth();
	}

	if (DamageEvent.DamageTypeClass == UCDamageType_LastCombo::StaticClass())
	{
		ComboCount = 0;
		Action->CancelSpecial();
		if (State->IsKnockOutMode() == false)
			bDamagedLastAttack = true;
		State->SetKnockOutMode();
		LauchUpValue = 1000.0f;
		return Status->GetHealth();
	}

	if (DamageEvent.DamageTypeClass == UCDamageType_Counter::StaticClass())
	{
		Action->CancelSpecial();
		State->SetStiffMode();
		return Status->GetHealth();
	}

	if (State->IsKnockOutMode() == false)
	{
		State->SetHittedMode();
		bDamagedLastAttack = false;
	}


	return Status->GetHealth();
}

void ACEnemy_Base::AboryByDamage()
{
	StopAnimMontage();
}

void ACEnemy_Base::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Idle: Idle();	break;
	case EStateType::Special: Special();	break;
	case EStateType::Hitted: Hitted();	break;
	case EStateType::Dead: Dead();	break;
	case EStateType::Taunt: Taunt();	break;
	case EStateType::Discover: DiscoverTarget(); break;
	case EStateType::WakeUp: WakeUp(); break;
	case EStateType::Stiff: Stiff(); break;
	}
}

void ACEnemy_Base::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (State->IsKnockOutMode() == true)
	{
		HittedResult = Hit;
		LaunchByHitted();
	}
}

void ACEnemy_Base::DamageUpdate(float amount, bool bDamage)
{
	if (bDamage)
	{
		Status->SubHealth(amount);
	}
	else
	{
		Status->AddHealth(amount);
	}

	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->SetVisible(true);
	UKismetSystemLibrary::K2_SetTimer(this, "OffHealthUI", HealthUIOnTime, false);
}

void ACEnemy_Base::OffHealthUI()
{
	Cast<UCUserWidget_HealthBar>(HealthWidget->GetUserWidgetObject())->SetVisible(false);
}

void ACEnemy_Base::Idle()
{
	BodyMaterial->SetScalarParameterValue("Intensity", 0.0f);
}

void ACEnemy_Base::Special()
{
	BodyMaterial->SetScalarParameterValue("Intensity", SpecialIntensity);
}

void ACEnemy_Base::Hitted()
{
	Montages->PlayHitted();
}

void ACEnemy_Base::Taunt()
{
	Montages->PlayTaunt();
}

void ACEnemy_Base::Stiff()
{
	Montages->PlayStiff();
	End_Popcorn();
}

void ACEnemy_Base::DiscoverTarget()
{
	Montages->PlayDiscover();
}

void ACEnemy_Base::Dead()
{
	CheckFalse(State->IsDeadMode());

	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	for (ACTargetPoint* target : Targets)
	{
		target->Destroy();
	}

	Montages->PlayDead();
}

void ACEnemy_Base::End_Hit()
{
	CheckTrue(State->IsKnockOutMode());

	State->SetIdleMode();
}

void ACEnemy_Base::End_Dead()
{
	Action->End_Dead();
	Destroy();
}

void ACEnemy_Base::End_Discover()
{
	State->SetIdleMode();
}

void ACEnemy_Base::End_Stiff()
{
	State->SetIdleMode();
}

void ACEnemy_Base::WakeUp()
{
	FixMesh();

	if (Montages->IsDataValid(EStateType::WakeUp) == true)
	{
		End_Popcorn();
		Montages->PlayWakeUp();
		return;
	}

	bUseLerpTurn = true;
}

void ACEnemy_Base::End_WakeUp()
{
	State->SetIdleMode();
}

void ACEnemy_Base::LaunchByHitted()
{
	CheckFalse(bDamagedLastAttack);
	bDamagedLastAttack = false;

	FVector up = FVector::UpVector;

	FVector Direction = HittedResult.ImpactNormal * LauchValue + up * LauchUpValue;

	/*
		현재 가해지는 양이,
		충격량이 이미 존재한다면, 양을 조절하는 것이 좋을듯 함
	*/

	if (FMath::IsNearlyZero(GetVelocity().Size(), 2.0f) == false)
	{
		Direction *= 0.5f;
	}

	LaunchCharacter(Direction, false, true);
}

void ACEnemy_Base::Popcorn()
{
	StopAnimMontage();

	GetMesh()->SetWorldLocation(GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetSimulatePhysics(true);
}

void ACEnemy_Base::End_Popcorn()
{
	bUseLerpTurn = false;
	FixMesh();
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
	GetMesh()->SetWorldTransform(GetActorTransform());

	GetMesh()->AddRelativeLocation(FVector(0, 0, -40));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}

void ACEnemy_Base::FixMesh()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetWorldLocation(GetActorLocation());

	GetMesh()->AddRelativeLocation(FVector(0, 0, -40));
}

void ACEnemy_Base::LerpTurn(float Delta)
{
	FRotator rotator = GetMesh()->GetComponentRotation();
	FRotator target = GetActorRotation() + FRotator(0, -90, 0);

	if (rotator.Equals(target, 0.2f) == true)
	{
		End_Popcorn();
		End_WakeUp();
		return;
	}

	rotator = UKismetMathLibrary::RInterpTo(rotator, target, Delta, LerpSpeed);

	GetMesh()->SetWorldRotation(rotator, false, nullptr, ETeleportType::TeleportPhysics);
}

void ACEnemy_Base::End_Taunt()
{
	State->SetIdleMode();
}
