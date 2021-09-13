#include "CActionComponent.h"
#include"Global.h"
#include<GameFramework/Character.h>
#include"Weapons/CWeaponData.h"
#include"Weapons/CEquipment.h"
#include"Weapons/CAction.h"
#include"Weapons/CAttachment.h"
#include"Weapons/CWeapon.h"
#include"Items/CEquipItem.h"
#include"Items/CEquipItem_Weapon.h"
#include"Items/CEquip_Arrow.h"
#include"Weapons/CAction_Bow.h"
#include"Characters/CEnemy_Base.h"

UCActionComponent::UCActionComponent()
{

}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		// 데이터 에셋이 꽂혀있는 녀석들만
		if (!!DataAssets[i])
		{
			DataAssets[i]->BeginPlay(character, &Datas[i]);
		}
	}

	// 이게 아님 -> Attachment class를 생성하는 것도 아이템 쪽에 있음
	EnemyBaseEquip();
}

void UCActionComponent::EnemyBaseEquip()
{
	CheckNull(Cast<ACEnemy_Base>(GetOwner()));
	ACAttachment* attachment = nullptr;
	FTransform transform;
	EActionType type = EActionType::Unarmed;

	// Datas 에 어차피 Attach가 안들어가는데?
	// WeaponData 쪽에서 가져와야 하나...???
	TSubclassOf<class ACAttachment> attachmentClass = DataAssets[(int32)type]->AttachmentClass;
	CheckNull(attachmentClass);
	/*
		실제로 장착을 해야하는 부분이 남아 있음
	*/

	attachment = GetOwner()->GetWorld()->SpawnActorDeferred<ACAttachment>(attachmentClass, transform, GetOwner());
	//attachment->SetActorLabel(GetLabelName("Attachment"));
	UGameplayStatics::FinishSpawningActor(attachment, transform);

	EquipAttach(type, attachment);
}

void UCActionComponent::OffAllCollision()
{
	for (UCWeapon* data : Datas)
	{
		if (!!data == false)	// 데이터 없음
			continue;

		if (!!data->GetAttachment() == false) // 무기에 장착된 정보 없음
			continue;

		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::OnChangeItem(UCEquipItem* NewItem, bool result)
{
	EActionType type = EActionType::Unarmed;
	ACAttachment* attach = nullptr;

	if (NewItem->EquipType == EEquipType::Arrow)
	{
		ACAction_Bow* bowAction = Cast<ACAction_Bow>(Datas[(int32)EActionType::Bow]->GetAction());
		UCEquip_Arrow* newItem = Cast< UCEquip_Arrow>(NewItem);
		if (result == true)
		{
			if (!!bowAction && !!newItem)
			{
				bowAction->SetArrowItem(newItem);
			}
		}
		else
		{
			bowAction->SetArrowItem(nullptr);
		}
		return;
	}

	if (NewItem->EquipType == EEquipType::MainWeapon)
	{
		if (Type == EActionType::Blade || Type == EActionType::Spear || Type == EActionType::TwoHand) SetUnarmedMode();

		UCEquipItem_Weapon* weaponItem = Cast<UCEquipItem_Weapon>(NewItem);
		switch (weaponItem->WeaponType)
		{
		case EMainWeapon::Blade:
			type = EActionType::Blade;
			break;
		case EMainWeapon::GreatSword:
			type = EActionType::TwoHand;
			break;
		case EMainWeapon::Spear:
			type = EActionType::Spear;
			break;
		}
	}

	if (NewItem->EquipType == EEquipType::Bow)
	{
		type = EActionType::Bow;
	}

	if (result == true)
	{
		attach = NewItem->GetAttachment();

		//if (Type != type)
		//{
		//	SetUnarmedMode();		// Weapon Changes, Set Unarmed Mode
		//}

		Datas[(int32)type]->GetAction()->SetEquipValue(NewItem->GetItemValue());
	}
	else if(Type == type)
	{
		SetUnarmedMode();
	}

	CLog::Print(result ? "Equip" : "UnEquip");
	CLog::Print((int)type);

	AttachBind(type, attach);
}

void UCActionComponent::AttachBind(EActionType type, ACAttachment* attach)
{
	// main weapon 이라면 main weapon에 해당하는 녀석들을 nullptr로
	if (type == EActionType::Blade || type == EActionType::Spear || type == EActionType::TwoHand)
	{
		if (!!Datas[1])	Datas[1]->SetAttachment(nullptr);
		if (!!Datas[2])	Datas[2]->SetAttachment(nullptr);
		if (!!Datas[3])	Datas[3]->SetAttachment(nullptr);
	}

	EquipAttach(type, attach);
}

void UCActionComponent::EquipAttach(EActionType type, ACAttachment* attach)
{
	Datas[(int32)type]->SetAttachment(attach);

	// attach 자체가 nullptr 이면 equip 쪽의 delegate를 해제 시킴
	if (attach == nullptr)
	{
		Datas[(int32)type]->GetEquipment()->OnEquipmentDelegate.Clear();
		Datas[(int32)type]->GetEquipment()->OnUnequipmentDelegate.Clear();
		return;
	}

	if (!!Datas[(int32)type]->GetEquipment())
	{
		Datas[(int32)type]->GetEquipment()->OnEquipmentDelegate.AddDynamic(attach, &ACAttachment::OnEquip);
		Datas[(int32)type]->GetEquipment()->OnUnequipmentDelegate.AddDynamic(attach, &ACAttachment::OnUnequip);
	}

	if (!!Datas[(int32)type]->GetAction())
	{
		if (attach->OnAttachmentBeginOverlap.IsBound() == false)
			attach->OnAttachmentBeginOverlap.AddDynamic(Datas[(int32)type]->GetAction(), &ACAction::OnAttachmentBeginOverlap);

		if (attach->OnAttachmentEndOverlap.IsBound() == false)
			attach->OnAttachmentEndOverlap.AddDynamic(Datas[(int32)type]->GetAction(), &ACAction::OnAttachmentEndOverlap);
	}
}

void UCActionComponent::SetUnarmedMode()
{
	// 현재 들고 있는 무기 타입이 존재시
	if (!!Datas[(int32)Type])
		Datas[(int32)Type]->GetEquipment()->Unequip();

	Datas[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetBladeMode()
{
	SetMode(EActionType::Blade);
}

void UCActionComponent::SetSpearMode()
{
	SetMode(EActionType::Spear);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetBowMode()
{
	SetMode(EActionType::Bow);
}

void UCActionComponent::SetLoadMode()
{
	SetMode(EActionType::Load);
}

void UCActionComponent::DoAction()
{
	// owner check 하여 Enemy_base를 상속 받는 녀석이라면 가능하도록?
	//if(GetOwner() )
	//CLog::Print(GetOwner()->GetName());
	//CheckTrue(IsUnarmedMode());	// 무기를 들고 있을 때만?


	if (!!Datas[(int32)Type])
	{
		ACAction* action = Datas[(int32)Type]->GetAction();

		if (!!action)
		{
			action->DoAction();
		}
	}
}

void UCActionComponent::DoAim_Begin()
{
}

void UCActionComponent::DoAim_End()
{
}

void UCActionComponent::DoSpecial()
{
	if (!!Datas[(int32)Type])
	{
		ACAction* action = Datas[(int32)Type]->GetAction();

		if (!!action)
		{
			action->DoSpecial();
		}
	}
}

void UCActionComponent::CancelSpecial()
{
	if (!!Datas[(int32)Type])
	{
		ACAction* action = Datas[(int32)Type]->GetAction();

		if (!!action)
		{
			action->CancelSpecial();
		}
	}
}

void UCActionComponent::Dead()
{
	OffAllCollision();
}

void UCActionComponent::End_Dead()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i] && !!Datas[i]->GetAttachment())
		{
			Datas[i]->GetAttachment()->Destroy();
		}

		if (!!Datas[i] && !!Datas[i]->GetEquipment())
		{
			Datas[i]->GetEquipment()->Destroy();
		}

		if (!!Datas[i] && !!Datas[i]->GetAction())
		{
			Datas[i]->GetAction()->Destroy();
		}
	}
}

void UCActionComponent::AbortByDamage()
{
}


void UCActionComponent::SetMode(EActionType InType)
{
	//CheckTrue()

	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}

	// 무기에서 다른 무기로 바꾸려 함
	else if (IsUnarmedMode() == false)
	{
		if (!!Datas[(int32)Type])
		{
			Datas[(int32)Type]->GetEquipment()->Unequip();
		}
	}

	//// 다른 타입에 장착 하는 무기가 있는가
	if (!!Datas[(int32)InType])
		Datas[(int32)InType]->GetEquipment()->Equip();

	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);
}