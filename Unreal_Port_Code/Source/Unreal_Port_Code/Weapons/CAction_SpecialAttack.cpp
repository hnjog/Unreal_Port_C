#include "CAction_SpecialAttack.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include"Components/CStateComponent.h"
#include"Components/CStatusComponent.h"
#include<Weapons/CDamageType_LastCombo.h>
#include<Weapons/CDamageType_SpecialAttack.h>
#include<Weapons/CSpecialPoint.h>
#include"Interfaces/ICharacter.h"

void ACAction_SpecialAttack::BeginPlay()
{
	Super::BeginPlay();

	FTransform transform;
	SpecialPoint = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACSpecialPoint>(ACSpecialPoint::StaticClass(), transform, OwnerCharacter);
	SpecialPoint->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

	UGameplayStatics::FinishSpawningActor(SpecialPoint, transform);

	SpecialPoint->OnSpecialBeginOverlap.AddDynamic(this, &ACAction_SpecialAttack::OnSpecialPointBeginOverlap);
	SpecialPoint->OnSpecialEndOverlap.AddDynamic(this, &ACAction_SpecialAttack::OnSpecialPointEndOverlap);
	SpecialPoint->SetGenHit();
}

void ACAction_SpecialAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAction_SpecialAttack::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	if (bEnableAttack == true)
	{
		bExistAttack = true;
		bEnableAttack = false;
		return;
	}

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
	CheckFalse(Datas.Num() > 0);

	CheckFalse(State->IsIdleMode());
	State->SetSpecialMode();

	if (bEnableSpecial == true)
	{
		bExistSpecial = true;
		bEnableSpecial = false;
		return;
	}

	SpecialPoint->OnCollision();

	//state가 Discover 이기에?
	CLog::Print("Data In", 1);

	OwnerCharacter->PlayAnimMontage(SpecialDatas[0].AnimMontage, SpecialDatas[0].PlayRate, SpecialDatas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACAction_SpecialAttack::CancelSpecial()
{
	Super::CancelSpecial();

	State->SetIdleMode();

	SpecialPoint->OffCollision();
}

void ACAction_SpecialAttack::Begin_Special()
{
	Super::Begin_DoAction();

	CheckFalse(bExistSpecial);
	bExistSpecial = false;

	OwnerCharacter->StopAnimMontage();
	IndexSpecial++;
	FMath::Clamp<int32>(IndexSpecial, 0, SpecialDatas.Num() - 1);

	SpecialHittedCharacters.Empty();

	OwnerCharacter->PlayAnimMontage(SpecialDatas[IndexSpecial].AnimMontage, SpecialDatas[IndexSpecial].PlayRate, SpecialDatas[IndexSpecial].StartSection);
	SpecialDatas[IndexSpecial].bCanMove ? Status->SetMove() : Status->SetStop();

}

void ACAction_SpecialAttack::End_Special()
{
	Super::End_Special();

	OwnerCharacter->StopAnimMontage(Datas[IndexAttack].AnimMontage);
	IndexAttack = 0;

	SpecialHittedCharacters.Empty();

	State->SetIdleMode();
	Status->SetMove();

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

	if (FMath::IsNearlyZero(hitStop) == false) 
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
	}

	UParticleSystem* hitEffect = Datas[IndexAttack].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[IndexAttack].EffectTransform;
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

	FPointDamageEvent e;

	if (IndexAttack == Datas.Num() - 1)
	{
		e.DamageTypeClass = UCDamageType_LastCombo::StaticClass();
	}

	if (FMath::IsNearlyZero(EquipValue) == true) EquipValue = 1.0f;

	InOtherCharacter->TakeDamage(Datas[IndexAttack].PowerRate * EquipValue, e, InAttacker->GetController(), InAttackCauser);
}

void ACAction_SpecialAttack::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);

}

void ACAction_SpecialAttack::OnSpecialPointBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{
	for (const ACharacter* other : SpecialHittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	SpecialHittedCharacters.Add(InOtherCharacter);

	float hitStop = SpecialDatas[IndexSpecial].HitStop;

	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
	}

	UParticleSystem* hitEffect = SpecialDatas[IndexSpecial].Effect;
	if (!!hitEffect)
	{
		FTransform transform = SpecialDatas[IndexSpecial].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	TSubclassOf<UCameraShake> shake = SpecialDatas[IndexSpecial].ShakeClass;
	if (!!shake)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)
			controller->PlayerCameraManager->PlayCameraShake(shake);
	}

	FPointDamageEvent e;

	if (IndexSpecial == SpecialDatas.Num() - 1)
	{
		e.DamageTypeClass = UCDamageType_SpecialAttack::StaticClass();
	}

	if (FMath::IsNearlyZero(EquipValue) == true) EquipValue = 1.0f;

	InOtherCharacter->TakeDamage(SpecialDatas[IndexSpecial].PowerRate * EquipValue, e, InAttacker->GetController(), InAttackCauser);
}

void ACAction_SpecialAttack::OnSpecialPointEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	SpecialHittedCharacters.Empty();	// TArray 비우기
}

void ACAction_SpecialAttack::ResetGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}