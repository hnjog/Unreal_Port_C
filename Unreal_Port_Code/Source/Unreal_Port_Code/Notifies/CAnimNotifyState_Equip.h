#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Equip.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCAnimNotifyState_Equip : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// BN �̺�Ʈ �̸�, ��� �������̹Ƿ� override
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
