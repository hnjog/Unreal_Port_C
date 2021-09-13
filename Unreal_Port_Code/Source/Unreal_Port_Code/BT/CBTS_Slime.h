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
	float SearchTime = 0.0f;	// player�� ������ ��쿡 ����ϸ� GetMaxAge ���� ���� ��쿡�� �ǽ� ����, �� ���Ŀ� �Ϲ� ��� ���·� ��ȯ

	bool bTaunt = false;
	bool bDiscover = false;
};

/*
	BTService Node��
	AI ������ ������Ʈ �ϴ� ��׶��� �۾��� �����ϵ��� ����

	���񽺴� ���� Ʈ���� �⺻ �бⰡ Ȱ��ȭ�� �� ���������,
	�۾��� �޸� ����� ��ȯ���� ������, ���� �帧�� �������� ������ �ټ� �����ϴ�.

	�Ϲ������� �������� �˻�(TickNode)�� �����ϰ� ����� Black Board�� �����մϴ�.

	���ڷ����� ��忡�� ����Ȯ�� ����� �ʿ��� ��� OnSearchStart ����� ����ϼ���
	�װ����� ����Ǵ� ��� �˻�� ��� �̷������ �մϴ�.

	task�� ���� ������� �й��ϴ°�?
	�ϴ� Service�� ��׶��� ������ ��ó�ϸ�,
	BT���� �б⸦ �¿� ���������� Task�� ���� ���� �����ϵ��� �ؾ� ��

	���� �бⰡ �ʿ��� �κп� ���Ͽ�,
	�б⸦ ���� ��, �ൿ(task)�� �ϵ��� �ؾ� ��

	�ν��� �ȵǴ� ���
	Team Id �� ���� �Ǿ����� �ǽ�����
	(���� ���µ� �� ���� ã��)

	210913
	�ؾ��� ��
	- ������ ���� �߰� 

*/