#include "CAction_Melee.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include"Components/CStateComponent.h"
#include"Components/CStatusComponent.h"
#include<Weapons/CDamageType_LastCombo.h>
#include<Weapons/CDamageType_AirCombo.h>
#include<Weapons/CDamageType_AirLast.h>
#include<Weapons/CDamageType_Counter.h>
#include<Weapons/CSpecialPoint.h>
#include"Interfaces/ICharacter.h"
#include"Characters/CPlayer.h"

void ACAction_Melee::BeginPlay()
{
	Super::BeginPlay();

	FTransform transform;
	GuardPoint = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACSpecialPoint>(ACSpecialPoint::StaticClass(), transform, OwnerCharacter);
	GuardPoint->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	UGameplayStatics::FinishSpawningActor(GuardPoint, transform);

	GuardPoint->OnGuardPointBeginOverlap.AddDynamic(this, &ACAction_Melee::OnGuardPointBeginOverlap);
	GuardPoint->OnGuardPointEndOverlap.AddDynamic(this, &ACAction_Melee::OnGuardPointEndOverlap);

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);
	OnAirCombo.BindUFunction(player, "End_AirCombo");
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
	CheckFalse(Datas.Num() > 0); // ������ �����ϴ°�

	if (bEnableAttack == true)
	{
		bExistAttack = true;
		bEnableAttack = false; // ��ȸ��
		return;
	}

	// �ܺο��� ĳ���Ͱ� Idle üũ�� �ϰ� ���� ���;� �ϱ� ��
	if (State->IsIdleMode() == true)
	{
		State->SetActionMode();

		OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
		Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
		//Datas[Index].bPawnControl ? Status-
	}
	else if (State->IsAirComboWaitMode() == true && bEnableAirCombo == true)
	{
		State->SetAirComboMode();
		OwnerCharacter->PlayAnimMontage(AirComboDatas[0].AnimMontage, AirComboDatas[0].PlayRate, AirComboDatas[0].StartSection);
	}
}

void ACAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExistAttack);
	bExistAttack = false;

	HittedCharacters.Empty();

	// ���� ��Ÿ�ָ� ���ߴ� �뵵
	if (State->IsAirComboMode() == true && bEnableAirCombo == true)
	{
		OwnerCharacter->StopAnimMontage();
		IndexAir++;
		FMath::Clamp<int32>(IndexAir, 0, AirComboDatas.Num() - 1);

		OwnerCharacter->PlayAnimMontage(AirComboDatas[IndexAir].AnimMontage, AirComboDatas[IndexAir].PlayRate, AirComboDatas[IndexAir].StartSection);
		AirComboDatas[IndexAir].bCanMove ? Status->SetMove() : Status->SetStop();

		if (IndexAir == AirComboDatas.Num() - 1)
		{
			bEnableAirCombo = false;
		}
	}
	else
	{
		OwnerCharacter->StopAnimMontage();
		IndexAttack++;
		FMath::Clamp<int32>(IndexAttack, 0, Datas.Num() - 1);

		OwnerCharacter->PlayAnimMontage(Datas[IndexAttack].AnimMontage, Datas[IndexAttack].PlayRate, Datas[IndexAttack].StartSection);
		Datas[IndexAttack].bCanMove ? Status->SetMove() : Status->SetStop();
	}
}

void ACAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	HittedCharacters.Empty();
	if (State->IsAirComboMode() == true)
	{
		OwnerCharacter->StopAnimMontage(AirComboDatas[IndexAir].AnimMontage);
		IndexAir = 0;
		bEnableAirCombo = true;

		if (OnAirCombo.IsBound()) OnAirCombo.Execute();
	}
	else
	{
		OwnerCharacter->StopAnimMontage(Datas[IndexAttack].AnimMontage);
		IndexAttack = 0;

		State->SetIdleMode();
		Status->SetMove();
	}
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

	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	//HittedCharacters.Add(InOtherCharacter);
	HittedCharacters.AddUnique(InOtherCharacter);

	float hitStop = 0.0f;
	if (State->IsAirComboMode() == false)
		hitStop = Datas[IndexAttack].HitStop;

	// float�� 0�� �ſ� �����Ѱ��� ���� (float�� ��Ȯ�� 0�� ���� �ε� �Ҽ��� ���̱⿡)
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop�� 0�� �ƴ� ���� ���ٴ� ��
	{
		//0.02�� ����� ��
		// �ð��� �����Ե�, �����Ե� �Ҽ� ����
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		CTimeDilationManager::SetGlobalTime(GetWorld(), HitStopRate);
		// �ð��� �������� �ߴٸ�, �������� �� (1)
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * HitStopRate, false);
		/*
		GetWorld()�� Ÿ�̸�, UKismeticsystemlibrary�� Ÿ�̸�
		2������ ������?

		GetWorld() ������ handle�� �ֱ⿡
		������ �ð� ������ ����

		k2 ������ handle�� �ʿ� ����
		(�Ͻ� ����, ����, pause ���� ���� ���ϰ�)
		(�׳� ���� �ð��� ������ �Լ��� call ��)

		1�� �ڿ� �ȴ� �ϴ���
		������ ��ŭ ������� �������� �ð� ����� ����
		( hitStop * 2e-2f)
		(delta ���� �������⿡)

		custom Time Dilation?
		- �ش� ������ �ð� �����ϴ� ��
		(ȸ�� ����Ʈ ó��, ��ü �ð��� ����������, ���ΰ��� �������� �ӵ��� �����°�)

		AActor�� ���� ���� (get,set �Լ�����)
		�׳� ������ ���

		*/
	}

	// Play Effect Particle
	UParticleSystem* hitEffect = Datas[IndexAttack].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[IndexAttack].EffectTransform;
		// ĳ������ ���� ��ǥ�� ������
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
	// �� �������� ���縦 �Ҽ� �ִ� ����ü
	//FDamageEvent e;
	FPointDamageEvent e;
	// FHitResult : ���� ����� ǥ�� ����� ����ִ� DamageEvent ver

	if (IndexAttack == Datas.Num() - 1)
	{
		e.DamageTypeClass = UCDamageType_LastCombo::StaticClass();
	}

	if (IndexAir == AirComboDatas.Num() - 1)
	{
		e.DamageTypeClass = UCDamageType_AirLast::StaticClass();
		InOtherCharacter->TakeDamage(AirComboDatas[IndexAir].PowerRate * EquipValue, e, InAttacker->GetController(), InAttackCauser);
		return;
	}
	else if (State->IsAirComboMode() == true)
	{
		e.DamageTypeClass = UCDamageType_AirCombo::StaticClass();
		InOtherCharacter->TakeDamage(AirComboDatas[IndexAir].PowerRate * EquipValue, e, InAttacker->GetController(), InAttackCauser);
		return;
	}

	if (FMath::IsNearlyZero(EquipValue) == true) EquipValue = 1.0f;

	InOtherCharacter->TakeDamage(Datas[IndexAttack].PowerRate * EquipValue, e, InAttacker->GetController(), InAttackCauser);
}

void ACAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);
}

void ACAction_Melee::OnGuardPointBeginOverlap(class AActor* DefenseTo, class AActor* InAttackCauser)
{
	ACharacter* inOtherCharacter = Cast<ACharacter>(InAttackCauser->GetOwner());
	CheckNull(inOtherCharacter);	// ���߿��� Guard�� ������Ѿ� ������?

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
	IICharacter* character = Cast<IICharacter>(OwnerCharacter);

	if (!!character)
	{
		character->Parrying(false);
		character->Guard(false);
	}

	StiffedCharacters.Empty();	// TArray ����
}

void ACAction_Melee::ResetGlobalDilation()
{
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	CTimeDilationManager::SetGlobalTime(GetWorld(), HitStopRate, true);
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
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop�� 0�� �ƴ� ���� ���ٴ� ��
	{
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		CTimeDilationManager::SetGlobalTime(GetWorld(), HitStopRate);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * HitStopRate, false);
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
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop�� 0�� �ƴ� ���� ���ٴ� ��
	{
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		CTimeDilationManager::SetGlobalTime(GetWorld(), HitStopRate);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * HitStopRate, false);
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