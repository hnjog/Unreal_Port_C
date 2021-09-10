#include "CItemFiled.h"
#include"Global.h"
#include"CItem.h"
#include<Components/StaticMeshComponent.h>
#include "Components/WidgetComponent.h"
#include<Widgets/CItemWidget.h>

ACItemFiled::ACItemFiled()
{
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bTickEvenWhenPaused = true; // ���� �ִ� �߿��� tick ���

	CHelpers::CreateComponent(this, &StaticMesh, "StaticMesh");
	CHelpers::CreateComponent(this, &NameWidget, "NameWidget", StaticMesh);

	TSubclassOf<UCItemWidget> NameWidgetClass;
	CHelpers::GetClass<UCItemWidget>(&NameWidgetClass, "WidgetBlueprint'/Game/Widzets/WBP_FiledItem.WBP_FiledItem_C'");
	NameWidget->SetWidgetClass(NameWidgetClass);
	NameWidget->SetDrawSize(FVector2D(160, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACItemFiled::SetItem(UCItem* item)
{
	Item = item;
}

void ACItemFiled::OnVisible()
{
	NameWidget->SetVisibility(true);
}

void ACItemFiled::UnActive()
{
	NameWidget->SetVisibility(false);
	Destroy();
}

void ACItemFiled::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(30.0f);

	StaticMesh->SetStaticMesh(Item->PickupMesh);

	for (int32 i = 0; i < StaticMesh->GetNumMaterials(); i++)
	{
		StaticMesh->SetMaterial(i, Item->PickupMesh->GetMaterial(i));
	}

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(true);
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionProfileName("Items");

	NameWidget->InitWidget();
	Cast<UCItemWidget>(NameWidget->GetUserWidgetObject())->SetText(Item->ItemDisplayName);

}


void ACItemFiled::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NameWidget->SetVisibility(false);
}

/*
	Item
	������ ��Ÿ���� ����
	player�� �ֿ�� �ִ� ���� ��,

	������� ����
	player�� ���� �ۿ� ������ ��
	(�ٵ� �̰� tick���� ��� filed �������� �����ϴ°� �³�?)

*/