#include "CTimeDilationManager.h"
#include<Global.h>

void CTimeDilationManager::SetGlobalTime(UObject* WorldContext, float Value, bool bExpend)
{
	/*
		GetTimeDilation을 이용하여 
		곱하여야 함

		bExpend가 false인 경우는
		그대로 기존 값에 곱하여 사용

		true인 경우는

		들어온 값을 역수로 취해 다시 곱해줌 (1- value)

	*/
	if (bExpend == true)
	{
		Value = 1 / Value;
	}

	Value = UGameplayStatics::GetGlobalTimeDilation(WorldContext) * Value;
	CLog::Print(Value, 4);

	UGameplayStatics::SetGlobalTimeDilation(WorldContext, Value);
}
