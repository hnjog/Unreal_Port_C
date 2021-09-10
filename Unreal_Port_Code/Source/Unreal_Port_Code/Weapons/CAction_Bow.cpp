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
	CheckFalse(ArrowItem->Count > 0);	// doaction�� ������ �κк��� �ᱹ ������ ArrowItem�� ������ٴ� ���ε�?

	//if (Aim->IsAvaliable())
	//	CheckFalse(Aim->IsInZoom());

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACAction_Bow::Begin_DoAction()
{
	ACAttachment* attach = Action->GetCurrentData()->GetAttachment();	// bow ������ �ϱ�
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
		�ϴ� ���������δ� ������,
		������ ȭ�� �ٽ��Ŀ� �߻� ��,

		-> �ϴ� �κ��丮 �� �׼� ������Ʈ���� unequip ����� ������� ��
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
	1. ���� ��
	special �� ���� ���̹Ƿ�
	DoSpecial
	- begin special
	- end special ���� ��ҵ��� �θ� ������ ��

	2. attachment �� socket �� �޾ƿ;� ��
	(ps, ���� bow�� ABP�� ����ؾ� �ϴ°�)

	3. Inventory�� arrow, count�� �޾ƿ�,
	1�� �̻��̸� ȭ���� �߻��� �� �־�� ��

	4. Ȱ�� ����
	������ ������ 1~1.5 �� �ϵ�, (ȭ�� ������ + Ȱ ������)�� ���Ͽ� ó������


	�÷��̾��� ȭ���� ������ ���?



*/