#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Aim.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCUserWidget_Aim : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnAim();

	UFUNCTION(BlueprintImplementableEvent)
		void OffAim();

	UFUNCTION(BlueprintImplementableEvent)
		void OnTarget();

	UFUNCTION(BlueprintImplementableEvent)
		void OffTarget();

};
