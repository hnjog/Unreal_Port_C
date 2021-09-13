#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_Slime.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCBTS_Slime : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTS_Slime();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float SearchTime = 0.0f;	// player가 없어진 경우에 사용하며 GetMaxAge 보다 적은 경우에는 의심 상태, 그 이후엔 일반 대기 상태로 전환

	bool bTaunt = false;
	bool bDiscover = false;
};

/*
	BTService Node는
	AI 지식을 업데이트 하는 백그라운드 작업을 수행하도록 설계

	서비스는 동작 트리의 기본 분기가 활성화될 때 실행되지만,
	작업과 달리 결과를 반환하지 않으며, 실행 흐름에 직접적인 영향을 줄수 없습니다.

	일반적으로 정기적인 검사(TickNode)를 수행하고 결과를 Black Board에 저장합니다.

	데코레이터 노드에서 사전확인 결과가 필요한 경우 OnSearchStart 기능을 사용하세요
	그곳에서 수행되는 모든 검사는 즉시 이루어져야 합니다.

	task에 일을 어느정도 분배하는가?
	일단 Service는 백그라운드 역할을 자처하며,
	BT에서 분기를 태워 나누어지는 Task를 통해 일을 수행하도록 해야 함

	따라서 분기가 필요한 부분에 한하여,
	분기를 나눈 뒤, 행동(task)를 하도록 해야 함

	인식이 안되는 경우
	Team Id 가 구현 되었는지 의심하자
	(적이 없는데 뭘 적을 찾어)

	210913
	해야할 일
	- 강공격 패턴 추가 

*/