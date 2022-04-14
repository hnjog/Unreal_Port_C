#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_HealthBar.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCUserWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthBar(float health, float maxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void SetVisible(bool value);
	
};
