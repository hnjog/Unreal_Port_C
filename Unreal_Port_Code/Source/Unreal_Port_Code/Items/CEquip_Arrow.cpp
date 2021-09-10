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
	���� ���� Equip version���� ����

	-> �ϴ� ���� ���θ� �˾ƾ�
	�ٸ� Ÿ���� ȭ���� �����Ǿ�����,
	������ �����ϴ� ȭ���� equip�� false�� ����� ����

	-> ȭ�� ���� count�� ���߸�,
	0�� �̸� ��� �Ұ���

	-> ���� 0���� ���� ������ ���� �Ǹ� Inventory���� ���ŵǾ�� ��

	bow �ʿ��� ������ �� �༮�� �����;� ��

*/