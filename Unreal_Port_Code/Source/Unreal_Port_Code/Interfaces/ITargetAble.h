#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITargetAble.generated.h"

UINTERFACE(MinimalAPI)
class UITargetAble : public UInterface
{
	GENERATED_BODY()
};

class UNREAL_PORT_CODE_API IITargetAble
{
	GENERATED_BODY()

public:
	virtual void TargetWidgetVisible(bool val) {}
	virtual bool IsTargetStiffed() { return false; }
	virtual bool IsTargetKnockOuted() { return false; }
	virtual bool IsTargetFalling() { return false; }
	virtual FVector TargetLocation() { return FVector::ZeroVector; }

	virtual void TargetBeaten() {}
	virtual void End_TargetBeaten() {}
};
