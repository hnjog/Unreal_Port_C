#include "CWeaponData.h"
#include"Global.h"
#include<GameFramework/Character.h>
#include "CEquipment.h"
#include "CAttachment.h"
#include "CAction.h"
#include"CWeapon.h"

void UCWeaponData::BeginPlay(class ACharacter* InOwnerCharacter, class UCWeapon** OutWeapon)
{
	FTransform transform;

	/*ACAttachment* Attachment = nullptr;
	if (!!AttachmentClass)
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		Attachment->SetActorLabel(GetLabelName(InOwnerCharacter, "Attachment"));
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}*/

	ACEquipment* Equipment = nullptr;
	// 해당 클래스가 존재할 때만 (이 기능이 필요없을수도 있으므로)
	if (!!EquipmentClass)
	{
		// spawn 예약
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		// 추후에 생성될 수 있는 녀석들을 쳐줌
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Equipment->SetData(EquipmentData);
		Equipment->SetActorLabel(GetLabelName(InOwnerCharacter, "Equipment"));
		// 그 다음 Spawn을 확정시킴, transform을 또 받는 이유? (사이에 낑긴다던가, G-Fighting을 방지하려고, 같은 위치면 처리를 해줌)
		UGameplayStatics::FinishSpawningActor(Equipment, transform);
		// 그렇게 데이터를 다 받은뒤, 이 녀석의 begin play가 호출될 것
		/*if (!!Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}*/
	}

	ACAction* Action = nullptr;
	if (!!ActionClass)
	{
		Action = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAction>(ActionClass, transform, InOwnerCharacter);
		Action->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Action->SetActionDatas(DoActionDatas);
		Action->SetSpecialDatas(DoSpecialDatas);
		Action->SetActorLabel(GetLabelName(InOwnerCharacter, "Action"));
		UGameplayStatics::FinishSpawningActor(Action, transform);

		if (!!Equipment)
		{
			// const 포인터 끼리 연결하여 변경은 하지 못하지만, 값이 변하는 것을 알수 있음
			// 포인터로 안 넘겨도 되지만 포인터가 실시간으로 값이 바뀐다는 점을 보장할 수 있음
			// delegate 방식이 아니긴 하지만, 이건 이거대로..
			Action->SetEquipped(Equipment->GetEquipped());
		}

		/*if (!!Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(Action, &ACAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(Action, &ACAction::OnAttachmentEndOverlap);
		}*/
	}

	*OutWeapon = NewObject<UCWeapon>();
	//(*OutWeapon)->Attachment = Attachment;	// 힙 할당한 것을 더블 포인터를 이용하여 넣어줌 => 값이 아닌 주소를 할당 받음
	(*OutWeapon)->Equipment = Equipment;
	(*OutWeapon)->Action = Action;
}


FString UCWeaponData::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
{
	FString name;
	name.Append(InOwnerCharacter->GetActorLabel());
	name.Append("_");
	name.Append(InName);
	name.Append("_");
	name.Append(GetName().Replace(L"DA_", L""));

	return name;
}
