#pragma once

#include "CoreMinimal.h"
#include "Items/CItem.h"
#include "CConsumeItem.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCConsumeItem : public UCItem
{
	GENERATED_BODY()
	
public:
	UCConsumeItem();

	virtual void Use(class AActor* Actor) override;

};
