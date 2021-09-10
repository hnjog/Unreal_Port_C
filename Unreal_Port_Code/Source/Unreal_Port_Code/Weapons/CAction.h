#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponData.h"
#include "CAction.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACAction : public AActor
{
	GENERATED_BODY()

public:
	// ActionData에서 Spawn 시키면서 넣어줄 데이터들
	FORCEINLINE void SetActionDatas(TArray<FActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetSpecialDatas(TArray<FSpecialData> InDatas) { SpecialDatas = InDatas; }
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }

	FORCEINLINE float GetEquipValue() { return EquipValue; }

	void SetEquipValue(float equipValue);

public:
	ACAction();

public:
	virtual void DoAction() {};
	
	virtual void Begin_DoAction() {};	// 노티파이용들
	virtual void End_DoAction() {};		// 필요없을수 있으므로 그냥 가상함수로

	virtual void DoSpecial() {};		// 이거 WeaponData 쪽에서 Special 관련 데이터 추가해야 할듯?
	virtual void CancelSpecial() {};
	virtual void Begin_Special() {};
	virtual void End_Special() {};

	virtual void OnAim() {};
	virtual void OffAim() {};

public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

protected:
	TArray<FActionData> Datas;
	TArray<FSpecialData> SpecialDatas;

	const bool* bEquipped;

	bool bSpecial;

	float EquipValue;
};


