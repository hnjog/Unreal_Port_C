#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CItemWidget.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetText(FText InName) { Name = InName; }

public:
	UPROPERTY(BlueprintReadOnly)
		FText Name;

};
