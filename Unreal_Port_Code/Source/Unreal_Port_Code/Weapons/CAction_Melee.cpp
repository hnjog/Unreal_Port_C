#include "CAction_Melee.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include"Components/CStateComponent.h"
#include"Components/CStatusComponent.h"
#include<Weapons/CDamageType_LastCombo.h>
#include<Weapons/CDamageType_Counter.h>
#include<Weapons/CGuardPoint.h>
#include"Interfaces/ICharacter.h"

void ACAction_Melee::BeginPlay()
{
	Super::BeginPlay();

	FTransform transform;
	GuardPoint = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACGuardPoint>(ACGuardPoint::StaticClass(), transform, OwnerCharacter);
	GuardPoint->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	UGameplayStatics::FinishSpawningActor(GuardPoint, transform);

	GuardPoint->OnGuardPointBeginOverlap.AddDynamic(this, &ACAction_Melee::OnGuardPointBeginOverlap);
	GuardPoint->OnGuardPointEndOverlap.AddDynamic(this, &ACAction_Melee::OnGuardPointEndOverlap);
}

void ACAction_Melee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnGuard == true)
	{
		GuardTimer += DeltaTime;
	}
}

void ACAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0); // 데이터 존재하는가

	if (bEnableAttack == true)
	{
		bExistAttack = true;
		bEnableAttack = false; // 일회성
		return;
	}

	// 외부에서 캐릭터가 Idle 체크를 하고 여기 들어와야 하긴 함
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
	//Datas[Index].bPawnControl ? Status-
}

void ACAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExistAttack);
	bExistAttack = false;

	// 현재 몽타주를 멈추는 용도
	OwnerCharacter->StopAnimMontage();
	IndexAttack++;
	FMath::Clamp<int32>(IndexAttack, 0, Datas.Num() - 1);

	OwnerCharacter->PlayAnimMontage(Datas[IndexAttack].AnimMontage, Datas[IndexAttack].PlayRate, Datas[IndexAttack].StartSection);
	Datas[IndexAttack].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage(Datas[IndexAttack].AnimMontage);
	IndexAttack = 0;

	State->SetIdleMode();
	Status->SetMove();
}

void ACAction_Melee::DoSpecial()
{
	Super::DoSpecial();

	CheckFalse(State->IsIdleMode());
	State->SetSpecialMode();

	//CLog::Print("OnSpecial");

	bOnGuard = true;

	GuardPoint->OnCollision();
}

void ACAction_Melee::CancelSpecial()
{
	Super::CancelSpecial();

	State->SetIdleMode();

	//CLog::Print("CancelSpecial");

	bOnGuard = false;
	bParrying = false;
	bSuccessGuard = false;
	GuardTimer = 0.0f;
	GuardPoint->OffCollision();

	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	if (!!character)
	{
		character->Parrying(false);
		character->Guard(false);
	}
}

void ACAction_Melee::Begin_Special()
{
	Super::Begin_Special();

}

void ACAction_Melee::End_Special()
{
	Super::End_Special();

	CancelSpecial();
	Status->SetMove();
}

void ACAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	// 슬라임 쪽에서 이게 호출되었을 가능성이 존재하지 않는가??

	// 다단 히트 방지
	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	HittedCharacters.Add(InOtherCharacter);

	// Hit Stop
	float hitStop = Datas[IndexAttack].HitStop;
	// float가 0에 매우 근접한가에 대해 (float는 정확히 0이 힘든 부동 소수점 값이기에)
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop에 0이 아닌 값이 들어갔다는 뜻
	{
		//0.02에 가까운 값
		// 시간을 느리게도, 빠르게도 할수 있음
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		// 시간을 느려지게 했다면, 돌려놔야 함 (1)
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
		/*
		GetWorld()의 타이머, UKismeticsystemlibrary의 타이머
		2가지의 차이점?

		GetWorld() 버전은 handle이 있기에
		세심한 시간 조정이 가능

		k2 버전은 handle이 필요 없음
		(일시 정지, 스톱, pause 등은 하지 못하고)
		(그냥 일정 시간이 지나면 함수를 call 함)

		1초 뒤에 된다 하더라도
		느려진 만큼 곱해줘야 정상적인 시간 계산이 가능
		( hitStop * 2e-2f)
		(delta 값이 느려지기에)

		custom Time Dilation?
		- 해당 액터의 시간 조절하는 것
		(회피 저스트 처럼, 전체 시간은 느려지지만, 주인공만 정상적인 속도로 때리는것)

		AActor에 속한 변수 (get,set 함수없음)
		그냥 접근해 사용

		*/
	}

	// Play Effect Particle
	UParticleSystem* hitEffect = Datas[IndexAttack].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[IndexAttack].EffectTransform;
		// 캐릭터의 상태 좌표가 더해진
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	// Play Camera Shake
	TSubclassOf<UCameraShake> shake = Datas[IndexAttack].ShakeClass;
	if (!!shake)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)
			controller->PlayerCameraManager->PlayCameraShake(shake);
	}

	// send Damage
	// 더 세부적인 묘사를 할수 있는 구조체
	//FDamageEvent e;
	FPointDamageEvent e;
	// FHitResult : 맞은 방향과 표면 충격이 들어있는 DamageEvent ver

	if (IndexAttack == Datas.Num() - 1)
	{
		e.DamageTypeClass = UCDamageType_LastCombo::StaticClass();
	}

	// 몬스터가 맨손 공격을 하는 경우에 대비
	if (FMath::IsNearlyZero(EquipValue) == true) EquipValue = 1.0f;

	// Apply Damage의 역할 (이 함수가 블프의 데미지 역할을 다 함)
	// 재정의도 가능하기에, Any Damage처럼 사용도 가능함
	InOtherCharacter->TakeDamage(Datas[IndexAttack].PowerRate * EquipValue, e, InAttacker->GetController(), InAttackCauser);
}

void ACAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	HittedCharacters.Empty();	// TArray 비우기
}

void ACAction_Melee::OnGuardPointBeginOverlap(class AActor* DefenseTo, class AActor* InAttackCauser)
{
	ACharacter* inOtherCharacter = Cast<ACharacter>(InAttackCauser->GetOwner());
	CheckNull(inOtherCharacter);	// 나중에는 Guard로 전향시켜야 할지도?

	//CLog::Print("OnEvent");

	// 이거 적의 Attachment를 가져와야 하는 것은 아닌지??
	// Attachment가 잡혔다는 것 자체가
	// 이미 ActionMode라는 반증이 아닐까??

	//UCStateComponent* anotherState = CHelpers::GetComponent<UCStateComponent>(inOtherCharacter);
	//CheckNull(anotherState);
	//CLog::Print("CheckActionMode");
	//CheckTrue(anotherState->IsActionMode());

	//CLog::Print("Attacked");

	IICharacter* character = Cast<IICharacter>(OwnerCharacter);

	if (GuardTimer <= 0.4f)
	{
		//CLog::Print("Parry");
		bParrying = true;
		OwnerCharacter->PlayAnimMontage(SpecialDatas[0].AnimMontage, SpecialDatas[0].PlayRate, SpecialDatas[0].StartSection);
		Parrying(Cast<ACharacter>(GetOwner()), this, inOtherCharacter);
		if (!!character)
			character->Parrying(true);
	}
	else
	{
		//CLog::Print("Guard");
		bSuccessGuard = true;
		OwnerCharacter->PlayAnimMontage(SpecialDatas[1].AnimMontage, SpecialDatas[1].PlayRate, SpecialDatas[1].StartSection);
		Guard();
		if (!!character)
			character->Guard(true);
	}

}

void ACAction_Melee::OnGuardPointEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	StiffedCharacters.Empty();	// TArray 비우기
}

void ACAction_Melee::ResetGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void ACAction_Melee::Parrying(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	for (const ACharacter* other : StiffedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	StiffedCharacters.Add(InOtherCharacter);

	float hitStop = SpecialDatas[0].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop에 0이 아닌 값이 들어갔다는 뜻
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
	}

	// Play Effect Particle
	UParticleSystem* hitEffect = SpecialDatas[0].Effect;
	if (!!hitEffect)
	{
		FTransform transform = SpecialDatas[0].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	// Play Camera Shake
	TSubclassOf<UCameraShake> shake = SpecialDatas[0].ShakeClass;
	if (!!shake)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)
			controller->PlayerCameraManager->PlayCameraShake(shake);
	}

	// send Damage
	FPointDamageEvent e;

	e.DamageTypeClass = UCDamageType_Counter::StaticClass();

	InOtherCharacter->TakeDamage(SpecialDatas[0].PowerRate, e, InAttacker->GetController(), InAttackCauser);
}

void ACAction_Melee::Guard()
{
	bSuccessGuard = true;
	float hitStop = SpecialDatas[1].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop에 0이 아닌 값이 들어갔다는 뜻
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
	}

	// Play Effect Particle
	UParticleSystem* hitEffect = SpecialDatas[1].Effect;
	if (!!hitEffect)
	{
		FTransform transform = SpecialDatas[1].EffectTransform;
		transform.AddToTranslation(GuardPoint->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	// Play Camera Shake
	TSubclassOf<UCameraShake> shake = SpecialDatas[1].ShakeClass;
	if (!!shake)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)
			controller->PlayerCameraManager->PlayCameraShake(shake);
	}
	CancelSpecial();
}