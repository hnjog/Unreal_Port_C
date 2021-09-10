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
	bool AddItem(class UCItem* Item);		// 아이템 추가
	bool RemoveItem(class UCItem* Item);	// 아이템 제거

	void ItemEquip(class UCEquipItem* EquipItem, bool result);

public:
	UFUNCTION(BlueprintCallable)
		void SortInven();		// 기본적으로는 인덱스 기준으로 할 예정 (필요에 의해 차후 추가)

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
	// 게임 시작시 가지고 있을 아이템
	// Instanced : 이 클래스의 인스턴스가 생성될 때, 이프로퍼티에 기본으로 할당된 오브젝트 사본을 쥐어줌
	// EditInline 및 Export로 간주됨
	UPROPERTY(EditDefaultsOnly, Instanced)
		TArray<class UCItem*> DefaultItems;

	// 소지하는 아이템
	// BlueprintReadOnly : 블프에서 읽을 수 있게 (변경x)
	// VisibleAnywhere : 모든 프로퍼티 창에 보이지만, 편집 x
	// 여기서 다중 배열로 만들어 아이템 타입에 따라 인벤토리를 나눌수 있지 않을까?
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
		TArray<class UCItem*> Items;

	// 용량
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		int32 Capacity;

	// BlueprintAssignable - 블프에서 접근 가능하게
	// delegate
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FInventoryUpdated OnInventoryUpdated;

	// purpose : 무기가 바뀔 시, player와 Action에 아이템을 전달하여,
	// 장착 상태, 액션 상태를 바꾸는 역할
	UPROPERTY(BlueprintAssignable, Category = "Equip")
		FEquipChanged OnEquipChanged;
};

/*
	Attachment가 바뀌었다는 것을 다른 녀석들도 알아야 함
*/
