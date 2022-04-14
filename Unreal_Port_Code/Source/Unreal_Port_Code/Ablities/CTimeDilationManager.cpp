#include "CTimeDilationManager.h"
#include<Global.h>

void CTimeDilationManager::SetGlobalTime(UObject* WorldContext, float Value, bool bExpend)
{
	/*
		GetTimeDilation�� �̿��Ͽ� 
		���Ͽ��� ��

		bExpend�� false�� ����
		�״�� ���� ���� ���Ͽ� ���

		true�� ����

		���� ���� ������ ���� �ٽ� ������ (1- value)

	*/
	if (bExpend == true)
	{
		Value = 1 / Value;
	}

	Value = UGameplayStatics::GetGlobalTimeDilation(WorldContext) * Value;
	CLog::Print(Value, 4);

	UGameplayStatics::SetGlobalTimeDilation(WorldContext, Value);
}
