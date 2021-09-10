#pragma once

#include "CoreMinimal.h"
#include "Items/CEquipItem.h"
#include "CEquipItem_Weapon.generated.h"

UENUM(BlueprintType)
enum class EMainWeapon : uint8
{
	Blade, GreatSword, Spear, Load, Max
};

UCLASS()
class UNREAL_PORT_CODE_API UCEquipItem_Weapon : public UCEquipItem
{
	GENERATED_BODY()

public:
	UCEquipItem_Weapon();

	virtual void Use(class AActor* Actor) override;

	virtual void BeginPlay() override;
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Equip")
		EMainWeapon WeaponType;

};
