#include "CInventoryComponent.h"
#include "CItem.h"
#include "Global.h"
#include"CEquipItem.h"
#include"CConsumeItem.h"
#include"Interfaces/IAbleInventory.h"
#include"Components/CActionComponent.h"

UCInventoryComponent::UCInventoryComponent()
{
	Capacity = 20;
}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}

bool UCInventoryComponent::AddItem(UCItem* Item)
{
	if (Items.Num() >= Capacity || !Item)
	{
		return false;
	}

	if (Item->bStackable == true)
	{
		for (auto& item : Items)
		{
			if (Item->Index == item->Index) //  && item->Count >= 1
			{
				item->Count++;
				OnInventoryUpdated.Broadcast();

				return true;
			}
		}
	}

	Item->OwningInventory = this;
	Item->World = GetWorld();
	Item->BeginPlay();
	Items.Emplace(Item);
	// vector 처럼 emplace 아주 약간 더 좋음 (성능)
	// add 가 가독성은 더 좋을수 있음

	//Update UI
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UCInventoryComponent::RemoveItem(UCItem* Item)
{
	if (!Item)
	{
		return false;
	}

	CLog::Print(Item->Count);

	if (Item->bStackable == true && Item->Count > 1)
	{
		Item->Count--;
		OnInventoryUpdated.Broadcast();
		return true;
	}

	Item->OwningInventory = nullptr;
	Item->World = nullptr;
	Item->OutTask();
	Items.RemoveSingle(Item);	// 배열에서 제거함
	OnInventoryUpdated.Broadcast();

	if (Item->bEquipable == true)
	{
		UCEquipItem* item = Cast<UCEquipItem>(Item);
		if (!!item)
		{
			PrintLine();
			ItemEquip(item, false);
		}
		
	}

	return true;
}

void UCInventoryComponent::ItemEquip(UCEquipItem* EquipItem, bool result)
{
	if (OnEquipChanged.IsBound())
		OnEquipChanged.Broadcast(EquipItem,result);

	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast();
}

void UCInventoryComponent::SortInven()
{
	Items.Sort([](const UCItem& A, const UCItem& B) {
		if (A.Type != B.Type)
		{
			return A.Type < B.Type;
		}

		if (A.bStackable == true && B.bStackable == true)
		{
			return A.Count < B.Count;
		}

		return A.Index < B.Index;
	});
}

/*
	tick component는 성능상 별로라는 말이 있음
*/