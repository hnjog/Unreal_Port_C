#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAction.h"
#include"Components/CActionComponent.h"
#include "CAction_Bow.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACAction_Bow : public ACAction
{
	GENERATED_BODY()

public:
	FORCEINLINE class UCAim* GetAim() { return Aim; }

	//void SetArrowItem(class UCEquip_Arrow* arrowItem);

private:
	UFUNCTION()
		void AbortByTypeChange(EActionType InPrevType, EActionType InNewType);

protected:
	virtual void BeginPlay() override;

public:
	ACAction_Bow();

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	// ������ ��������� �����ϱ⿡
	virtual void Tick(float DeltaTime) override;

	virtual void DoSpecial() override;
	virtual void CancelSpecial() override;
	virtual void Begin_Special() override;
	virtual void End_Special() override;

	virtual void AddCharge(float value) override;
	virtual void ReleaseSpecial() override;

	virtual void OnAim() override;
	virtual void OffAim() override;

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult, bool Charge);

	void ShootArrow(bool charge = false);

	void LineTraceCameraTarget(FVector startLocation, FVector cameraLocation, FRotator& outRotator);

private:
	UPROPERTY()
		class UCAim* Aim;

private:
	class UCActionComponent* Action;
	class UCAblityComponent* Ablity;
	class UCInventoryComponent* Inventory;
	class UCEquip_Arrow* ArrowItem;

	//UPROPERTY(VisibleAnywhere)
	//	TSubclassOf<class ACArrow> ArrowClass;

	// For Charge Shot
	bool bCharge = false;
	bool bSuccessCharge = false;
	float ChargeTime = 1.0f;
	float ChargeTimer = 0.0f;
	// ���� ���� ��, �������̶�� ���� CArrow�� ����

	/*
		charge�� �ϱ� ���Ͽ�

		Axis �� ���� �̺�Ʈ�� �����ؾ� �ҵ�?

		Axis �� ���� �Լ� ����
		+ Ȱ���� Special ������ �� �˾ƾ� ��

	*/

};
