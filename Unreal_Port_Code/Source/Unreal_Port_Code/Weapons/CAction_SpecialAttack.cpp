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

void ACAction_SpecialAttack::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExistAttack);
	bExistAttack = false;

	// 현재 몽타주를 멈추는 용도
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
	if (FMath::IsNearlyZero(hitStop) == false) // hitstop에 0이 아닌 값이 들어갔다는 뜻
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
	}

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

void ACAction_SpecialAttack::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);


}

void ACAction_SpecialAttack::OnGuardPointBeginOverlap(class AActor* DefenseTo, class AActor* InAttackCauser)
{
	ACharacter* inOtherCharacter = Cast<ACharacter>(InAttackCauser->GetOwner());
	CheckNull(inOtherCharacter);	// 나중에는 Guard로 전향시켜야 할지도?

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

	StiffedCharacters.Empty();	// TArray 비우기
}

void ACAction_SpecialAttack::ResetGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}