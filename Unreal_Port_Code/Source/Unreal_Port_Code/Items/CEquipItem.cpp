#include "CEquipItem.h"
#include"Global.h"
#include <GameFramework/Character.h>
#include"Items/CInventoryComponent.h"
#include"Characters/CPlayer.h"
#include "Weapons/CAttachment.h"

UCEquipItem::UCEquipItem()
{
	bEquipable = true;
	Type = EItemType::Equip;
}

void UCEquipItem::BeginPlay()
{
	Super::BeginPlay();
	bEquip = false;
	FTransform transform;

	if (!!AttachmentClass && Attachment == nullptr)
	{
		// �̰� ��ü�� �κ��丮�� ������ �� ȣ��Ǳ⿡ ������
		Attachment = OwningInventory->GetOwner()->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, OwningInventory->GetOwner());
		Attachment->SetActorLabel(GetLabelName("Attachment"));
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}

	SetVisible();
}

void UCEquipItem::Use(AActor* Actor)
{
	Super::Use(Actor);

	// bEquip �̿��ؼ� UI�ʿ��� ���� or ����
	bEquip = !bEquip;

	OwningInventory->ItemEquip(this, bEquip);
	SetVisible();

}

void UCEquipItem::SetEquip(bool equip)
{
	bEquip = equip;
	SetVisible();
}

FString UCEquipItem::GetLabelName(FString InName)
{
	FString name;
	name.Append(OwningInventory->GetOwner()->GetActorLabel());
	name.Append("_");
	name.Append(InName);
	name.Append("_");
	name.Append(GetName().Replace(L"DA_", L""));

	return name;
}

void UCEquipItem::OutTask()
{
	Super::OutTask();

	Attachment->Destroy();
	Attachment = nullptr;
}

void UCEquipItem::SetVisible()
{
	Attachment->SetActorHiddenInGame(!bEquip);
}
