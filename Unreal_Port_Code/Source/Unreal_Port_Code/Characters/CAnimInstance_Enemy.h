#pragma once

#include "CoreMinimal.h"
#include "Characters/CAnimInstance.h"
#include"CEnemy_AI.h"
#include "CAnimInstance_Enemy.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCAnimInstance_Enemy : public UCAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EEnemyStance StanceType;

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnStanceChanged(EEnemyStance InPrevStance, EEnemyStance InNewStance);

};

/*
	애니메이션 관련하여,
	detect 상태 가 되었을때,
	다시 플레이어를 발견하면,
	애니메이션 재생 후 battle 상태로,

	그렇지 않고, (GetMaxAge로 인해 인지가 풀린 경우)
	None 상태로 전환한다면,
	Taunt 몽타주를 재생 후, idle 상태로

	근데 결국,
	battle 상태 등을 판별하려면, 여기서 해야 함

	state?
	-> 행동 상태를 판별하는 것임
	(idle, action 등 단발적인 행동들)

	behavior?
	-> 역시 BT에서 행동으로 판별하는 녀석들임
	다만 BTS에서 bool 값 등을 이용할 수는 있을 듯?
	-> 차라리 BTS의 bool 값을 이용하는게 나을수도 있고,
	(다만 적의 상태를 처리할 거라면 ACEnemy_AI에 정의하는게 더 괜찮을 듯 싶은데)


*/