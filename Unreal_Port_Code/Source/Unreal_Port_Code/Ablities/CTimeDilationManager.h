#pragma once

#include "CoreMinimal.h"

class UNREAL_PORT_CODE_API CTimeDilationManager
{
public:
	static void SetGlobalTime(UObject* WorldContext, float Value, bool bExpend = false);

};
