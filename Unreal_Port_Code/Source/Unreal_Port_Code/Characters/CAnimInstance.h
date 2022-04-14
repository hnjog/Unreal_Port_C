#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool Aim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool Falling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EStateType StateType;

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
};
