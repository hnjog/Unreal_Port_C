#include "CAction_SpecialAttack.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include"Components/CStateComponent.h"
#include"Components/CStatusComponent.h"
#include<Weapons/CDamageType_LastCombo.h>
#include<Weapons/CDamageType_Counter.h>
#include<Weapons/CGuardPoint.h>
#include"Interfaces/ICharacter.h"

void ACAction_SpecialAttack::BeginPlay()
{
	Super::BeginPlay();

	FTransform transform;
	GuardPoint = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACGuardPoint>(ACGuardPoint::StaticClass(), transform, OwnerCharacter);
	GuardPoint->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	UGameplayStatics::FinishSpawningActor(GuardPoint, transform);

	GuardPoint->OnGuardPointBeginOverlap.AddDynamic(this, &ACAction_SpecialAttack::OnGuardPointBeginOverlap);
	GuardPoint->OnGuardPointEndOverlap.AddDynamic(this, &ACAction_SpecialAttack::OnGuardPointEndOverlap);
}

void ACAction_SpecialAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAction_SpecialAttack::DoAction()
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
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
	//Datas[Index].bPawnControl ? Status-
}

void ACAction_SpecialAttack::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExistAttack);
	bExistAttack = false;

	// ���� ��Ÿ�ָ� ���ߴ� �뵵
	OwnerCharacter->StopAnimMontage();
	IndexAttack++;
	FMath::Clamp<int32>(IndexAttack, 0, Datas.Num() - 1);

	HittedCharacters.Empty();

	OwnerCharacter->PlayAnimMontage(Datas[IndexAttack].AnimMontage, Datas[IndexAttack].PlayRate, Datas[IndexAttack].StartSection);
	Datas[IndexAttack].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACAction_SpecialAttack::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage(Datas[IndexAttack].AnimMontage);
	IndexAttack = 0;

	HittedCharacters.Empty();

	State->SetIdleMode();
	Status->SetMove();
}


void ACAction_SpecialAttack::DoSpecial()
{
	Super::DoSpecial();

	CheckFalse(State->IsIdleMode());
	State->SetSpecialMode();

	GuardPoint->OnCollision();
}

void ACAction_SpecialAttack::CancelSpecial()
{
	Super::CancelSpecial();

	State->SetIdleMode();

	GuardPoint->OffCollision();

	
}

void ACAction_SpecialAttack::Begin_Special()
{
	Super::Begin_Special();

}

void ACAction_SpecialAttack::End_Special()
{
	Super::End_Special();

	CancelSpecial();
	Status->SetMove();
}

void ACAction_SpecialAttack::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	HittedCharacters.Add(InOtherCharacter);

	float hitStop = Datas[IndexAttack].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop�� 0�� �ƴ� ���� ���ٴ� ��
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
	}

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

	// ���Ͱ� �Ǽ� ������ �ϴ� ��쿡 ���
	if (FMath::IsNearlyZero(EquipValue) == true) EquipValue = 1.0f;

	// Apply Damage�� ���� (�� �Լ��� ������ ������ ������ �� ��)
	// �����ǵ� �����ϱ⿡, Any Damageó�� ��뵵 ������
	InOtherCharacter->TakeDamage(Datas[IndexAttack].PowerRate * EquipValue, e, InAttacker->GetController(), InAttackCauser);
}

void ACAction_SpecialAttack::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);


}

void ACAction_SpecialAttack::OnGuardPointBeginOverlap(class AActor* DefenseTo, class AActor* InAttackCauser)
{
	ACharacter* inOtherCharacter = Cast<ACharacter>(InAttackCauser->GetOwner());
	CheckNull(inOtherCharacter);	// ���߿��� Guard�� ������Ѿ� ������?

	IICharacter* character = Cast<IICharacter>(OwnerCharacter);


}

void ACAction_SpecialAttack::OnGuardPointEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	IICharacter* character = Cast<IICharacter>(OwnerCharacter);

	if (!!character)
	{
		character->Parrying(false);
		character->Guard(false);
	}

	StiffedCharacters.Empty();	// TArray ����
}

void ACAction_SpecialAttack::ResetGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}