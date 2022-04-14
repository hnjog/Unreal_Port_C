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

	// sort �� + ���� ���� �뵵
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		int32 Index;

	// �������� �� �� �ִ� ��
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		bool bStackable;

	// ����� �� �ִ°�?
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

	// weight of item (��������� ���� ����)
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
	virtual void Use(class AActor* Actor) {};	// ���


	////BI - �������� �����ϵ��� (������Ʈ�� �����ϴ� �͵� ����ؾ� �ϱ� �ϴµ�.., event ó�� ������ ���� ���� ����)
	UFUNCTION(BlueprintImplementableEvent)
		void OnUse(class AActor* Actor);

	// �κ��丮���� ������ ����� 
	UFUNCTION(BlueprintCallable)
		void OutInventory();

	// �κ��丮�� �� �� ȣ��ǵ���
	virtual void BeginPlay() {};

	// �κ��丮���� ���� �� ȣ��� (�ı��Ǳ� �� ȣ��)
	virtual void OutTask() {};

};
