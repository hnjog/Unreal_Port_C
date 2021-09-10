#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAbleInventory.generated.h"

UINTERFACE(MinimalAPI)
class UIAbleInventory : public UInterface
{
	GENERATED_BODY()
};

class UNREAL_PORT_CODE_API IIAbleInventory
{
	GENERATED_BODY()

public:
	virtual void OnUseItem(class UCItem* item) {}

	virtual void SetEquipItem(class UCEquipItem* EquipItem, bool result) {}
};
