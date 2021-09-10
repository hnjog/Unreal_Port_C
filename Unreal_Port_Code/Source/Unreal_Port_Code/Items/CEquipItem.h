#pragma once

#include "CoreMinimal.h"
#include "Items/CItem.h"
#include "CEquipItem.generated.h"

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	MainWeapon, Shiled, Bow, Arrow, Defend, Max
};

UCLASS()
class UNREAL_PORT_CODE_API UCEquipItem : public UCItem
{
	GENERATED_BODY()

public:
	UCEquipItem();

	virtual void BeginPlay() override;
	virtual void OutTask() override;

	virtual void Use(class AActor* Actor) override;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE bool GetEquip() { return bEquip; }

	void SetEquip(bool equip);

private:
	FString GetLabelName(FString InName);

protected:
	void SetVisible();

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equip")
		EEquipType EquipType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;


protected:
	class ACAttachment* Attachment;

	UPROPERTY(BlueprintReadOnly)
		bool bEquip = false;
};

// Blade, GreatSword, Spear, Load, Shiled, Bow, Defend, Max
// MainWeapon

// Weapon Data를 여기에 넣는 것도 고려

// 데이터 에셋으로 만들수 있으면 베스트긴 함
