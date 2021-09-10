#include "CEquip_Arrow.h"
#include"Global.h"
#include <GameFramework/Character.h>
#include<Items/CInventoryComponent.h>
#include "Weapons/CAttachment.h"
#include<Components/StaticMeshComponent.h>

UCEquip_Arrow::UCEquip_Arrow()
{
	bStackable = true;
	bEquipable = true;
	EquipType = EEquipType::Arrow;
}

void UCEquip_Arrow::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(Attachment);
	if (!!mesh)
	{
		mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UCEquip_Arrow::Use(AActor* Actor)
{
	Super::Use(Actor);
}
/*
	돌고 돌아 Equip version으로 만듦

	-> 일단 장착 여부를 알아야
	다른 타입의 화살이 장착되었을때,
	기존에 존재하는 화살의 equip을 false로 만들수 있음

	-> 화살 사용시 count를 낮추며,
	0개 이면 사용 불가함

	-> 또한 0개인 순간 장착이 해제 되며 Inventory에서 제거되어야 함

	bow 쪽에서 장착된 이 녀석을 가져와야 함

*/