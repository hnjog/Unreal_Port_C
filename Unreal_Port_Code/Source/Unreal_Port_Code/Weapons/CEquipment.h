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
	void Equip_Implementation();	// ��������� �Ƚᵵ ������ ������ �� -> _Implementation���� BN�� ��ü�� ����� ����

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip(); //Socket? Particle? Decal? -> Delegate Execute (����ü�δ� ����)
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
	// Purpose : Set (EActionType)Mode �� Attachment(���� ����)�� ������ �����ϴ� �뵵
	// Call : Begin_Equip�� ��Ƽ���̿��� ȣ��� ��
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
	bool bEquipped;		// �ǽð����� �ѱ� ����
	FEquipmentData Data;
};
