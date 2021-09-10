#include "CConsumeItem.h"
#include"Global.h"
#include <GameFramework/Character.h>
#include<Items/CInventoryComponent.h>

UCConsumeItem::UCConsumeItem()
{
	bStackable = true;
	Type = EItemType::Consume;
}

void UCConsumeItem::Use(class AActor* Actor)
{
	// Inventory �� ������ �ִ���
	UCInventoryComponent* inven = CHelpers::GetComponent<UCInventoryComponent>(Actor);
	CheckNull(inven);

	//Count--;
	OwningInventory->RemoveItem(this);

}