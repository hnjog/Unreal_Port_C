#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equip, Consume ,Max
};

UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class UNREAL_PORT_CODE_API UCItem : public UObject
{
	GENERATED_BODY()

public:
	UCItem();

	UPROPERTY()
		class UWorld* World;

	// equip, use type
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText UseActionText;

	// sort 용 + 도감 등의 용도
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		int32 Index;

	// 여러개를 들 수 있는 지
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		bool bStackable;

	// 장비할 수 있는가?
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		bool bEquipable;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		uint8 Count;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		EItemType Type;

	// display mesh
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		class UStaticMesh* PickupMesh;

	// thumbnail
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		class UTexture2D* Thumbnail;

	//display name in inventory
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText ItemDisplayName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item", meta = (MultiLine = true))
		FText ItemDescription;

	// weight of item (사용하지는 않을 거임)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item", meta = (ClampMin = 0.0))
		float Weight;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ItemValue = 0.0f;
public:
	UPROPERTY()
		class UCInventoryComponent* OwningInventory;

public:
	FORCEINLINE virtual class UWorld* GetWorld() const { return World; }
	FORCEINLINE float GetItemValue() const { return ItemValue; }

public:
	// AInventorySystemCharacter
	virtual void Use(class AActor* Actor) {};	// 사용


	////BI - 블프에서 구현하도록 (컴포넌트로 구현하는 것도 고려해야 하긴 하는데.., event 처럼 가상은 여기 들어갈수 없나)
	UFUNCTION(BlueprintImplementableEvent)
		void OnUse(class AActor* Actor);

	// 인벤토리에서 꺼낼때 사용할 
	UFUNCTION(BlueprintCallable)
		void OutInventory();

	// 인벤토리에 들어갈 때 호출되도록
	virtual void BeginPlay() {};

	// 인벤토리에서 나갈 때 호출됨 (파괴되기 전 호출)
	virtual void OutTask() {};

};
