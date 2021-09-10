#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Blade, Spear, TwoHand, Bow, Load, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PORT_CODE_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();


private:
	UPROPERTY(EditDefaultsOnly)
		class UCWeaponData* DataAssets[(int32)EActionType::Max];

public:
	FORCEINLINE class UCWeapon* GetCurrentData() { return Datas[(int32)Type]; }

	// weapon

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBladeMode() { return Type == EActionType::Blade; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsSpearMode() { return Type == EActionType::Spear; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBowMode() { return Type == EActionType::Bow; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsLoadMode() { return Type == EActionType::Load; }

	void OffAllCollision();

	// �������� �ٲ������, �ش� �������� Ÿ�Կ� ���� ���ε��� �ٲپ��� ����
	UFUNCTION()
		void OnChangeItem(class UCEquipItem* NewItem, bool result);

public:
	void SetUnarmedMode();
	void SetBladeMode();
	void SetSpearMode();
	void SetTwoHandMode();
	void SetBowMode();
	void SetLoadMode();

public:
	void DoAction();

	void DoAim_Begin();
	void DoAim_End();

	void DoSpecial();
	void CancelSpecial();

	void Dead();
	void End_Dead();

	void AbortByDamage();

private:
	// Enemy�� ȣ���� �� �ִ� �Լ��� ����� �δ� ���� ���� ������ (�ƴ� �׳� ���ο��� �˻�����)
	void EnemyBaseEquip();

	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);
	void AttachBind(EActionType type, class ACAttachment* attach);
	void EquipAttach(EActionType type, class ACAttachment* attach);

protected:
	virtual void BeginPlay() override;

public:
	//Purpose : ���⿡ ���� ���� ���� ����
	//Call : Set000Mode ȣ�� ��
	//Bind : ex) ACAnimInstance::OnTypeChanged
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType Type;

	UPROPERTY()
		class UCWeapon* Datas[(int32)EActionType::Max];

};

/*
	��Ŭ���� �׼�

	��Ŭ���� �����

	�� �׼� ����(����)�� ���� �ൿ�� �� ������

*/