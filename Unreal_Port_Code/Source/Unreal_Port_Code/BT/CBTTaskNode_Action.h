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
	// Delay���� Ŀ���� �����Ű�� succeed �� ��ȯ�ϴ� �뵵
	float RunningTime = 0.0f;

};

/*
	Enemy ������ ���ǰ� �׿� ���� ����

	- �������� �ϴ� �Ÿ� (�Ϲ� ���� ~ ������ �Ÿ�)
	(������ ���� ����Ī �ϸ鼭 �Ÿ� ��� ���� ����?)

	�ش� �Ÿ� �����̸�?
	
	-> ������??
	���Ͱ� �ð� �Ǹ鼭
	���� ���� ����� ���̱� �ѵ�...

	��� Action���� ��ĥ ������?
	
	�׷���,
	AI�� ���� �� ���������
	�ൿ�� �� �ִ�

	ex ������ ���� ���鼭 ȭ������ �÷��̾�� ��� ����
	�ܼ��� Action �ϳ��� �ƴ϶�,

	�̵��� ���� ������ ���� �� ��
	-> �ᱹ ���� AI�� Phase ��� ������ ������ �����ؾ� �ҵ�
	-> Action�� ���� Effect�� �ΰ� ȿ�� ���� �����ϴ� ����

	0. Behavior�� ������ Enum �κ� �߰�

	1. Behavior ���� ������ ��,
	�ش� ���¿� ����
	�Ǵ�
	(AI Controller�� Behavior �Ÿ���
	Distance �� 2�� ���� �߰��� ����,
	������ �� special)

	2. Action�� Index�� �Ǵ��ϴ� ����� �̿��Ͽ�
	������ �� Index�� �ø� ������ �����Ͽ� �����ϱ�

	�ٸ� ���� ���� ������ �ϴ� ���� ������?

	Index�� �Ǻ��ϸ� �ȵɵ� �ѵ�...

	-> ������ Notify�� �߰�?
	-> �̰� ���� ����ϱ� ��

	�׳� Special�� �ִ� ���� ���� ������ ����
	
	player�� LagDoll�� ��� �ִϸ��̼� ���� ����

	Action�� Special ����

	���� Enemy ������ ���� ��,
	player ��� ���� �����ϱ�

*/