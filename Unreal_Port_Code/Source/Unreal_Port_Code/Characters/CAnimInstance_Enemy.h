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
	�ִϸ��̼� �����Ͽ�,
	detect ���� �� �Ǿ�����,
	�ٽ� �÷��̾ �߰��ϸ�,
	�ִϸ��̼� ��� �� battle ���·�,

	�׷��� �ʰ�, (GetMaxAge�� ���� ������ Ǯ�� ���)
	None ���·� ��ȯ�Ѵٸ�,
	Taunt ��Ÿ�ָ� ��� ��, idle ���·�

	�ٵ� �ᱹ,
	battle ���� ���� �Ǻ��Ϸ���, ���⼭ �ؾ� ��

	state?
	-> �ൿ ���¸� �Ǻ��ϴ� ����
	(idle, action �� �ܹ����� �ൿ��)

	behavior?
	-> ���� BT���� �ൿ���� �Ǻ��ϴ� �༮����
	�ٸ� BTS���� bool �� ���� �̿��� ���� ���� ��?
	-> ���� BTS�� bool ���� �̿��ϴ°� �������� �ְ�,
	(�ٸ� ���� ���¸� ó���� �Ŷ�� ACEnemy_AI�� �����ϴ°� �� ������ �� ������)


*/