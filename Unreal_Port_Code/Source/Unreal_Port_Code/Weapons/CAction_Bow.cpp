#include "CAction_Bow.h"
#include "Global.h"
#include"GameFramework/Character.h"
#include"Components/CStateComponent.h"
#include"Components/CStatusComponent.h"
#include"Items/CInventoryComponent.h"
#include "CAim.h"
#include"Items/CItem_Arrow.h"
#include"CAttachment.h"
#include<Components/SkeletalMeshComponent.h>
#include"Items/CEquip_Arrow.h"

void ACAction_Bow::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	Action->OnActionTypeChanged.AddDynamic(this, &ACAction_Bow::AbortByTypeChange);

	Inventory = CHelpers::GetComponent<UCInventoryComponent>(OwnerCharacter);

}

void ACAction_Bow::SetArrowItem(UCEquip_Arrow* arrowItem)
{
	ArrowItem = arrowItem;
}

void ACAction_Bow::DoAction()
{
	CheckNull(ArrowItem);
	CheckFalse(ArrowItem->Count > 0);	// doaction이 들어오는 부분부터 결국 내부의 ArrowItem은 상관없다는 뜻인데?

	//if (Aim->IsAvaliable())
	//	CheckFalse(Aim->IsInZoom());

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACAction_Bow::Begin_DoAction()
{
	ACAttachment* attach = Action->GetCurrentData()->GetAttachment();	// bow 상태인 니까
	CheckNull(attach);
	USkeletalMeshComponent* skeletal = CHelpers::GetComponent<USkeletalMeshComponent>(attach);
	CheckNull(skeletal);

	FVector location = skeletal->GetSocketLocation("ArrowStart");
	FRotator rotator = OwnerCharacter->GetActorForwardVector().ToOrientationRotator();

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	ACItem_Arrow* arrowObject = GetWorld()->SpawnActorDeferred<ACItem_Arrow>
		(
			ACItem_Arrow::StaticClass(),
			transform,
			OwnerCharacter,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
	arrowObject->OnThrowBeginOverlap.AddDynamic(this, &ACAction_Bow::OnThrowBeginOverlap);
	arrowObject->SetItem(ArrowItem);
	arrowObject->SetEffect(ArrowItem->GetEffect());
	arrowObject->SetEffectTransform(ArrowItem->GetEffectTransform());
	arrowObject->SetArrowType(ArrowItem->GetArrowType());
	UGameplayStatics::FinishSpawningActor(arrowObject, transform);

	/*
		일단 개수적으로는 맞으나,
		아직도 화살 다쏜후에 발사 됨,

		-> 일단 인벤토리 및 액션 컴포넌트에서 unequip 제대로 시켜줘야 함
	*/

	Inventory->RemoveItem(ArrowItem);
}

void ACAction_Bow::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
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

void ACAction_Bow::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].PowerRate, e, OwnerCharacter->GetController(), this);

}

void ACAction_Bow::AbortByTypeChange(EActionType InPrevType, EActionType InNewType)
{
	CheckFalse(Aim->IsAvaliable());
	CheckFalse(Aim->IsInZoom());
	Aim->Off();
}

/*
	1. 줌인 은
	special 에 넣을 것이므로
	DoSpecial
	- begin special
	- end special 같은 요소들을 부모에 만들어야 함

	2. attachment 의 socket 을 받아와야 함
	(ps, 정말 bow는 ABP를 사용해야 하는가)

	3. Inventory의 arrow, count를 받아와,
	1개 이상이면 화살을 발사할 수 있어야 함

	4. 활의 경우는
	데미지 배율을 1~1.5 로 하되, (화살 데미지 + 활 데미지)를 곱하여 처리하자


	플레이어의 화살을 얻어오는 방법?



*/