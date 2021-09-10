#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipChanged, UCEquipItem*, EquipItem, bool, result);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PORT_CODE_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool AddItem(class UCItem* Item);		// ������ �߰�
	bool RemoveItem(class UCItem* Item);	// ������ ����

	void ItemEquip(class UCEquipItem* EquipItem, bool result);

public:
	UFUNCTION(BlueprintCallable)
		void SortInven();		// �⺻�����δ� �ε��� �������� �� ���� (�ʿ信 ���� ���� �߰�)

	template <typename T>
	void SearchAll(TArray<T>& OutArray)
	{
		for (auto Item : Items)
		{
			T comp = Cast<T>(Item);
			if (!!comp) OutArray.Add(comp);
		}
	}

public:
	// ���� ���۽� ������ ���� ������
	// Instanced : �� Ŭ������ �ν��Ͻ��� ������ ��, ��������Ƽ�� �⺻���� �Ҵ�� ������Ʈ �纻�� �����
	// EditInline �� Export�� ���ֵ�
	UPROPERTY(EditDefaultsOnly, Instanced)
		TArray<class UCItem*> DefaultItems;

	// �����ϴ� ������
	// BlueprintReadOnly : �������� ���� �� �ְ� (����x)
	// VisibleAnywhere : ��� ������Ƽ â�� ��������, ���� x
	// ���⼭ ���� �迭�� ����� ������ Ÿ�Կ� ���� �κ��丮�� ������ ���� ������?
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
		TArray<class UCItem*> Items;

	// �뷮
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		int32 Capacity;

	// BlueprintAssignable - �������� ���� �����ϰ�
	// delegate
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FInventoryUpdated OnInventoryUpdated;

	// purpose : ���Ⱑ �ٲ� ��, player�� Action�� �������� �����Ͽ�,
	// ���� ����, �׼� ���¸� �ٲٴ� ����
	UPROPERTY(BlueprintAssignable, Category = "Equip")
		FEquipChanged OnEquipChanged;
};

/*
	Attachment�� �ٲ���ٴ� ���� �ٸ� �༮�鵵 �˾ƾ� ��
*/
