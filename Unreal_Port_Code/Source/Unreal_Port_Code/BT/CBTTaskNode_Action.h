#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "AI")
		float AttackDelay = 2.0f;

public:
	UCBTTaskNode_Action();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Delay보다 커지면 종료시키며 succeed 를 반환하는 용도
	float RunningTime = 0.0f;

};

/*
	Enemy 강공격 조건과 그에 따른 구현

	- 강공격을 하는 거리 (일반 공격 ~ 강공격 거리)
	(예전에 무기 스위칭 하면서 거리 재는 것을 참고?)

	해당 거리 내부이며?
	
	-> 강공격??
	몬스터가 늘게 되면서
	패턴 등이 고려될 것이긴 한데...

	모두 Action으로 퉁칠 것인지?
	
	그래도,
	AI의 상태 및 페이즈에따라
	행동할 수 있는

	ex 보스가 빙빙 돌면서 화염구를 플레이어에게 쏘는 것은
	단순히 Action 하나가 아니라,

	이동과 공격 조건이 만족 된 것
	-> 결국 보스 AI는 Phase 라는 상위의 개념이 존재해야 할듯
	-> Action에 따른 Effect와 부가 효과 등을 구현하는 것임

	0. Behavior에 강공격 Enum 부분 추가

	1. Behavior 것을 가져온 뒤,
	해당 상태에 따라서
	판단
	(AI Controller의 Behavior 거리에
	Distance 를 2개 정도 추가할 생각,
	강공격 및 special)

	2. Action의 Index를 판단하는 방법을 이용하여
	강공격 은 Index를 늘린 것으로 판정하여 공격하기

	다만 차후 연속 공격을 하는 적에 있으면?

	Index로 판별하면 안될듯 한데...

	-> 강공격 Notify를 추가?
	-> 이게 제일 깔끔하긴 함

	그냥 Special에 넣는 것이 좋지 않을까 싶음
	
	player의 LagDoll과 기상 애니메이션 삽입 이후

	Action의 Special 구현

	이후 Enemy 강공격 구현 후,
	player 기상 상태 시험하기

*/