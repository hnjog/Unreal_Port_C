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
		// 이거 자체는 인벤토리가 생성된 뒤 호출되기에 괜찮음
		Attachment = OwningInventory->GetOwner()->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, OwningInventory->GetOwner());
		Attachment->SetActorLabel(GetLabelName("Attachment"));
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}

	SetVisible();
}

void UCEquipItem::Use(AActor* Actor)
{
	Super::Use(Actor);

	// bEquip 이용해서 UI쪽에서 강조 or 해제
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
