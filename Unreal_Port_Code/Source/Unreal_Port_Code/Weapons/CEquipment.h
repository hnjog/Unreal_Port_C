#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegate);

UCLASS()
class UNREAL_PORT_CODE_API ACEquipment : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetData(FEquipmentData InData) { Data = InData; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }

public:
	ACEquipment();

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip(); //PlayEquipMontage, PawnControl, ChangeBodyMaterial
	void Equip_Implementation();	// 헤더에서는 안써도 되지만 에러가 뜸 -> _Implementation으로 BN의 본체를 만들수 있음

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip(); //Socket? Particle? Decal? -> Delegate Execute (구조체로는 무리)
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();	 //State -> Idle
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();		// OrientRotation -> true
	void Unequip_Implementation();

protected:
	virtual void BeginPlay() override;

public:
	// Purpose : Set (EActionType)Mode 시 Attachment(무기 액터)의 소켓을 변경하는 용도
	// Call : Begin_Equip이 노티파이에서 호출될 때
	// Bind : Attachment::OnEquip -> AttachToComponent
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	bool bEquipped;		// 실시간으로 넘길 예정
	FEquipmentData Data;
};
