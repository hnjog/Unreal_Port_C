#pragma once

#include "CoreMinimal.h"
#include "Items/CEquipItem.h"
#include "CEquip_Arrow.generated.h"

UENUM(BlueprintType)
enum class EArrowType : uint8
{
	Wood, Fire, Max
};

UCLASS()
class UNREAL_PORT_CODE_API UCEquip_Arrow : public UCEquipItem
{
	GENERATED_BODY()

public:
	FORCEINLINE class UParticleSystem* GetEffect() { return HitEffect; }
	FORCEINLINE FTransform GetEffectTransform() { return HitEffectTransform; }
	FORCEINLINE EArrowType GetArrowType() { return ArrowType; }

private:
	UPROPERTY(EditAnywhere)
		class UParticleSystem* HitEffect;	// ÀûÀÌ ºÎµúÇûÀ» ¶§ÀÇ ÀÌÆåÆ® -> Àü´Þ ¿ë

	UPROPERTY(EditAnywhere)
		FTransform HitEffectTransform;

public:
	UPROPERTY(EditAnywhere)
		EArrowType ArrowType;

public:
	UCEquip_Arrow();

	virtual void BeginPlay() override;

	virtual void Use(class AActor* Actor) override;
};
