#include "CEquipItem_Weapon.h"
#include"Global.h"
#include <GameFramework/Character.h>
#include"Items/CInventoryComponent.h"
#include"Weapons/CAttachment.h"

UCEquipItem_Weapon::UCEquipItem_Weapon()
{
	bEquipable = true;
	EquipType = EEquipType::MainWeapon;
	Type = EItemType::Equip;
}

void UCEquipItem_Weapon::BeginPlay()
{
	Super::BeginPlay();
	SetVisible();
}

void UCEquipItem_Weapon::Use(AActor* Actor)
{
	Super::Use(Actor);
}
