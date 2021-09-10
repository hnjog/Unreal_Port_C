#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Taunt.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCBTTaskNode_Taunt : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "AI")
		float Delay = 1.0f;

public:
	UCBTTaskNode_Taunt();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// Delay보다 커지면 종료시키며 succeed 를 반환하는 용도
	float RunningTime = 0.0f;

};
