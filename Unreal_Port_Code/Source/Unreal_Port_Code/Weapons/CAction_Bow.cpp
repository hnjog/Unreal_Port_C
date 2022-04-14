#include "CAction_Bow.h"
#include "Global.h"
#include"GameFramework/Character.h"
#include"Components/CStateComponent.h"
#include"Components/CAblityComponent.h"
#include"Components/CStatusComponent.h"
#include"Items/CInventoryComponent.h"
#include "CAim.h"
#include"Items/CItem_Arrow.h"
#include"CAttachment.h"
#include<Components/SkeletalMeshComponent.h>
#include"Items/CEquip_Arrow.h"
#include"CArrow.h"
#include "Camera/PlayerCameraManager.h"
#include<Interfaces/ITargetAble.h>

ACAction_Bow::ACAction_Bow()
{
	//CHelpers::GetClass<ACArrow>(&ArrowClass, "Blueprint'/Game/WeaponAction/BP_CArrow.BP_CArrow_C'");
}

void ACAction_Bow::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	Action->OnActionTypeChanged.AddDynamic(this, &ACAction_Bow::AbortByTypeChange);
	Ablity = CHelpers::GetComponent<UCAblityComponent>(OwnerCharacter);

	Inventory = CHelpers::GetComponent<UCInventoryComponent>(OwnerCharacter);
}

//void ACAction_Bow::SetArrowItem(UCEquip_Arrow* arrowItem)
//{
//	ArrowItem = arrowItem;
//}

void ACAction_Bow::DoAction()
{
	//CheckNull(ArrowItem);
	//CheckFalse(ArrowItem->Count > 0);	// doaction이 들어오는 부분부터 결국 내부의 ArrowItem은 상관없다는 뜻인데?

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACAction_Bow::Begin_DoAction()
{
	ShootArrow();
}

void ACAction_Bow::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

// 우클릭을 누르면
// 몽타주 재생 -> 재생 이후 special 상태로
// 차지샷으로 만드는 것이 좋을듯?
// 차지샷 -> 관통 + 이펙트

void ACAction_Bow::DoSpecial()
{
	Super::DoSpecial();

	CheckFalse(State->IsIdleMode());
	State->SetSpecialMode();

	if (Aim->IsAvaliable())
		Aim->On();

	OwnerCharacter->PlayAnimMontage(SpecialDatas[0].AnimMontage, SpecialDatas[0].PlayRate, SpecialDatas[0].StartSection);
}

void ACAction_Bow::CancelSpecial()
{
	Super::CancelSpecial();

	Aim->Off();
	State->SetIdleMode();
}

void ACAction_Bow::Begin_Special()
{
	Super::Begin_Special();

	//State->SetSpecialMode();
}

void ACAction_Bow::End_Special()
{
	CancelSpecial();
	Status->SetMove();
}

//player가 우클릭을 놓을 때 발생할 이벤트
// 우클릭을 놓았을 때 생성할 화살 등을 고려해야 함

void ACAction_Bow::AddCharge(float value)
{
	Super::AddCharge(value);

	ChargeTimer += value;

	if (ChargeTimer >= ChargeTime)
	{
		bSuccessCharge = true;
	}
	CLog::Print(ChargeTimer, 3);
}

void ACAction_Bow::ReleaseSpecial()
{
	Super::ReleaseSpecial();

	CheckFalse(State->IsSpecialMode());

	OwnerCharacter->PlayAnimMontage(SpecialDatas[1].AnimMontage, SpecialDatas[1].PlayRate, SpecialDatas[1].StartSection);

	if (bSuccessCharge == true)
	{
		//CLog::Print("Success Charge");
		ShootArrow(true);
	}
	else
	{
		//CLog::Print("Just Charge");
		ShootArrow();
	}

	ChargeTimer = 0.0f;
	bSuccessCharge = false;

	End_Special();
}

void ACAction_Bow::Tick(float DeltaTime)
{
	Aim->Tick(DeltaTime);
}

void ACAction_Bow::OnAim()
{
	Aim->On();
}

void ACAction_Bow::OffAim()
{
	Aim->Off();
}

void ACAction_Bow::OnThrowBeginOverlap(FHitResult InHitResult, bool Charge)
{
	UParticleSystem* hitEffect = SpecialDatas[1].Effect;
	if (!!hitEffect)
	{
		FTransform transform = SpecialDatas[1].EffectTransform;
		// 캐릭터의 상태 좌표가 더해진
		//transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		transform.AddToTranslation(InHitResult.Actor->GetTransform().GetTranslation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	FDamageEvent e;
	float DamageValue;
	if (Charge == true)
	{
		DamageValue = 1.5f * SpecialDatas[1].PowerRate;
		IITargetAble* target = Cast<IITargetAble>(InHitResult.GetActor());
		if (!!target && (target->IsTargetStiffed() || target->IsTargetKnockOuted()))
		{
			DamageValue *= 2.5f;
		}
	}
	else
	{
		DamageValue = Datas[0].PowerRate;
	}

	InHitResult.GetActor()->TakeDamage(DamageValue * EquipValue, e, OwnerCharacter->GetController(), this);
}

void ACAction_Bow::AbortByTypeChange(EActionType InPrevType, EActionType InNewType)
{
	CheckFalse(Aim->IsAvaliable());
	CheckFalse(Aim->IsInZoom());
	Aim->Off();
}

void ACAction_Bow::ShootArrow(bool charge)
{
	ACAttachment* attach = Action->GetCurrentData()->GetAttachment();	// bow 상태인 니까
	CheckNull(attach);
	USkeletalMeshComponent* skeletal = CHelpers::GetComponent<USkeletalMeshComponent>(attach);
	CheckNull(skeletal);

	FVector location = skeletal->GetSocketLocation("ArrowStart");
	FRotator rotator = OwnerCharacter->GetActorForwardVector().ToOrientationRotator();
	if (charge == true)
	{
		// 카메라의 라인트레이스 기용을 이용하는 것은?
		FRotator temp;
		FVector cameralocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		LineTraceCameraTarget(location, cameralocation, temp);

		rotator = temp;
	}

	FTransform transform = SpecialDatas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	//CheckNull(ArrowClass);

	ACArrow* arrowObject = GetWorld()->SpawnActorDeferred<ACArrow>(ACArrow::StaticClass(), transform, OwnerCharacter);

	arrowObject->OnThrowOverlap.AddDynamic(this, &ACAction_Bow::OnThrowBeginOverlap);
	if (charge == true)
	{
		arrowObject->SetCharged();
	}
	UGameplayStatics::FinishSpawningActor(arrowObject, transform);
}

void ACAction_Bow::LineTraceCameraTarget(FVector startLocation, FVector cameraLocation, FRotator& outRotator)
{
	bool bFind = false;
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	FHitResult hitResult;

	FVector cameraEnd = cameraLocation + UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector() * 5000.0f;

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		cameraLocation, cameraEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		ignores,
		EDrawDebugTrace::ForOneFrame,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (!!Ablity->GetTargetActor() && Ablity->IsTargeting() == true)
	{
		outRotator = UKismetMathLibrary::FindLookAtRotation(startLocation, Ablity->GetTargetActor()->GetActorLocation());
		return;
	}

	if (hitResult.IsValidBlockingHit() == false)
	{
		outRotator = UKismetMathLibrary::FindLookAtRotation(startLocation, cameraEnd);
		return;
	}

	outRotator = UKismetMathLibrary::FindLookAtRotation(startLocation, hitResult.Location);

}