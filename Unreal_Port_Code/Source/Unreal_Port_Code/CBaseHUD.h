#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CBaseHUD.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACBaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableDraw() { bDraw = true; }
	FORCEINLINE void DisableDraw() { bDraw = false; }

private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* Texture;

public:
	ACBaseHUD();

	virtual void DrawHUD() override;

private:
	bool bDraw;
};
