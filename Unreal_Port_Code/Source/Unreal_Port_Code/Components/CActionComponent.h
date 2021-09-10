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

	// 아이템이 바뀌었을때, 해당 아이템의 타입에 따라 바인딩을 바꾸어줄 예정
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
	// Enemy가 호출할 수 있는 함수를 만들어 두는 것이 차라리 나을듯 (아니 그냥 내부에서 검사하자)
	void EnemyBaseEquip();

	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);
	void AttachBind(EActionType type, class ACAttachment* attach);
	void EquipAttach(EActionType type, class ACAttachment* attach);

protected:
	virtual void BeginPlay() override;

public:
	//Purpose : 무기에 따른 블렌드 포즈 변경
	//Call : Set000Mode 호출 시
	//Bind : ex) ACAnimInstance::OnTypeChanged
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType Type;

	UPROPERTY()
		class UCWeapon* Datas[(int32)EActionType::Max];

};

/*
	좌클릭은 액션

	우클릭은 스페셜

	각 액션 상태(무기)에 따른 행동을 할 예정임

*/