#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_FocusUI.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCUserWidget_FocusUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateFocus(float focus, float maxFocus);

	UFUNCTION(BlueprintImplementableEvent)
		void SetVisible(bool value);
};
