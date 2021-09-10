#include "CItem.h"
#include"Global.h"
#include"CItemFiled.h"
#include"CInventoryComponent.h"
#include<GameFramework/Character.h>

UCItem::UCItem()
{
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
	Count = 1;
}

void UCItem::OutInventory()
{
	FTransform transform;
	transform.SetLocation(OwningInventory->GetOwner()->GetActorLocation() + OwningInventory->GetOwner()->GetActorForwardVector() * 2.0f);
	ACItemFiled* filedItem = nullptr;

	filedItem = World->SpawnActorDeferred<ACItemFiled>(ACItemFiled::StaticClass(), transform,nullptr);
	filedItem->SetItem(this);
	UGameplayStatics::FinishSpawningActor(filedItem, transform);

	OwningInventory->RemoveItem(this);
}
