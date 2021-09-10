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
	// �ش� Ŭ������ ������ ���� (�� ����� �ʿ�������� �����Ƿ�)
	if (!!EquipmentClass)
	{
		// spawn ����
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		// ���Ŀ� ������ �� �ִ� �༮���� ����
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Equipment->SetData(EquipmentData);
		Equipment->SetActorLabel(GetLabelName(InOwnerCharacter, "Equipment"));
		// �� ���� Spawn�� Ȯ����Ŵ, transform�� �� �޴� ����? (���̿� ����ٴ���, G-Fighting�� �����Ϸ���, ���� ��ġ�� ó���� ����)
		UGameplayStatics::FinishSpawningActor(Equipment, transform);
		// �׷��� �����͸� �� ������, �� �༮�� begin play�� ȣ��� ��
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
			// const ������ ���� �����Ͽ� ������ ���� ��������, ���� ���ϴ� ���� �˼� ����
			// �����ͷ� �� �Ѱܵ� ������ �����Ͱ� �ǽð����� ���� �ٲ�ٴ� ���� ������ �� ����
			// delegate ����� �ƴϱ� ������, �̰� �̰Ŵ��..
			Action->SetEquipped(Equipment->GetEquipped());
		}

		/*if (!!Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(Action, &ACAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(Action, &ACAction::OnAttachmentEndOverlap);
		}*/
	}

	*OutWeapon = NewObject<UCWeapon>();
	//(*OutWeapon)->Attachment = Attachment;	// �� �Ҵ��� ���� ���� �����͸� �̿��Ͽ� �־��� => ���� �ƴ� �ּҸ� �Ҵ� ����
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
