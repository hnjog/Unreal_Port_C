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
	// Delegate �� Hit�� ���ͼ�, LastAttack�� ����ϴ� ����...?
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

		// Hit Animation�� ����Ǿ� , Idle�� ���ƿͼ� Wakeup�� �ȵǴ� ����...??
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
	// �ǰ� �� ��Ÿ�ָ� ���ߵ���
	// ���� �Ƹ� ���� ����ϴ� ����?
	// �� ��� ����ġ�� �ǰ� ġ ���� ����ؾ� �Ѵٰ� ������
	// ���� ���� TakeDamage �ʿ� �־ �ǰ�
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
	// ������ ��Ҹ� �־�� ��
	// �̰� ����Ѵٸ� super�� ���� �������� ȣ���ؾ� �ҵ�
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

	// ����� �и� ����� �߰��ϰ� �̻�����
	/*
		1. normal�� �̻��ϰ� ����

		2. ������ ������ �Ǿ� ������ ��
		- �̰� �ƴ�

		3. Popcorn ���³� �ٸ� ������ �̻�..??
		- �̰� �ǵ����� �ʾҴµ�?

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

	// ���⼭ ������ ��ġ�� Tick���� �������ָ� ���� ������??
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

	// x���� ������� ������ ������ �� �� ������ �ٲٴ°� ������ �ѵ�
	// �׳� rotator ������ üũ�ϴ°� ������
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
