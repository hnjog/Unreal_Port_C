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

ACEnemy_Base::ACEnemy_Base()
	:bDamagedLastAttack(false), WakeUpTimer(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");

}

void ACEnemy_Base::BeginPlay()
{
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_Base::OnStateTypeChanged);
	// Delegate 로 Hit만 얻어와서, LastAttack때 사용하는 것은...?
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemy_Base::OnComponentHit);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

	Super::BeginPlay();
}

void ACEnemy_Base::Tick(float DeltaTime)
{
	if (bUseLerpTurn == true)
	{
		LerpTurn(DeltaTime);
	}

	if (State->IsKnockOutMode() == true) //  && GetCharacterMovement()->IsFalling() == false
	{
		Popcorn();

		CheckTrue(GetCharacterMovement()->IsFalling());

		CLog::Print("IS falling", 2);
		CLog::Print(GetVelocity(), 3);
		CLog::Print(GetVelocity().Size(), 4);

		// Hit Animation이 재생되어 , Idle로 돌아와서 Wakeup이 안되는 것은...??
		WakeUpTimer += DeltaTime;

		CLog::Print(WakeUpTimer, 5);

		if (WakeUpTimer >= WakeUpTime && FMath::IsNearlyZero(GetVelocity().Size()) == true)
		{
			CLog::Print("WakeUp_Start", 6);
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

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	if (DamageEvent.DamageTypeClass == UCDamageType_LastCombo::StaticClass())
	{
		State->SetKnockOutMode();
		bDamagedLastAttack = true;
		return Status->GetHealth();
	}

	if (DamageEvent.DamageTypeClass == UCDamageType_Counter::StaticClass())
	{
		State->SetStiffMode();
		return Status->GetHealth();
	}

	State->SetHittedMode();
	bDamagedLastAttack = false;


	return Status->GetHealth();
}

void ACEnemy_Base::AboryByDamage()
{
	// 피격 시 몽타주를 멈추도록
	// 슈퍼 아머 등을 고려하는 것은?
	// 그 경우 경직치나 피격 치 등을 고려해야 한다고 생각함
	// 이쪽 말고 TakeDamage 쪽에 넣어도 되고
	StopAnimMontage();
}

void ACEnemy_Base::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
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
	//CLog::Print(OtherActor->GetName());
	HittedResult = Hit;

	if (State->IsKnockOutMode())
	{
		LaunchByHitted();
	}
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

	Montages->PlayDead();
}

void ACEnemy_Base::End_Hit()
{
	CheckTrue(State->IsKnockOutMode());

	State->SetIdleMode();
}

void ACEnemy_Base::End_Dead()
{
	// 지워줄 요소를 넣어야 함
	// 이걸 사용한다면 super를 가장 마지막에 호출해야 할듯
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

	// 가드와 패링 기능을 추가하고 이상해짐
	/*
		1. normal이 이상하게 측정

		2. 여러번 들어오게 되어 증폭이 됨
		- 이건 아님

		3. Popcorn 상태나 다른 상태의 이상..??
		- 이건 건들이지 않았는데?

	*/

	bDamagedLastAttack = false;

	FVector up = FVector::UpVector;

	FVector Direction = HittedResult.ImpactNormal * LauchValue + up * LauchUpValue;

	CLog::Print(Direction, 1);

	LaunchCharacter(Direction, false, true);
}

void ACEnemy_Base::Popcorn()
{
	// Popcorn
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAnimMontage();
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// 여기서 액터의 위치로 Tick마다 조정해주면 되지 않을까??
	if (GetCharacterMovement()->IsFalling() == true)
	{
		GetMesh()->SetWorldLocation(GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		SetActorLocation(GetMesh()->GetComponentLocation());
	}

	GetMesh()->SetSimulatePhysics(true);
}

void ACEnemy_Base::End_Popcorn()
{
	bUseLerpTurn = false;
	FixMesh();
	//GetMesh()->AttachTo(GetCapsuleComponent());
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
	CLog::Print("LerpIn!", 7);

	FRotator rotator = GetMesh()->GetComponentRotation();
	FRotator target = GetActorRotation() + FRotator(0, -90, 0);

	CLog::Print(rotator, 8);

	// x쪽은 상관없고 오히려 끝났을 때 그 값으로 바꾸는게 나을듯 한데
	// 그냥 rotator 값으로 체크하는게 나을듯
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
