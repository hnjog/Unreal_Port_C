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

	void SetArrowItem(class UCEquip_Arrow* arrowItem);

private:
	UFUNCTION()
		void AbortByTypeChange(EActionType InPrevType, EActionType InNewType);

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	// 에임을 자유자재로 적용하기에
	virtual void Tick(float DeltaTime) override;

	virtual void OnAim() override;
	virtual void OffAim() override;

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult);

private:
	UPROPERTY()
		class UCAim* Aim;

private:
	class UCActionComponent* Action;
	class UCInventoryComponent* Inventory;
	class UCEquip_Arrow* ArrowItem;
};
