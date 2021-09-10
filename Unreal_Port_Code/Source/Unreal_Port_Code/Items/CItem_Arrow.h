#pragma once

#include "CoreMinimal.h"
#include "Items/CItemFiled.h"
#include "CEquip_Arrow.h"
#include "CItem_Arrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowBeginOverlap, FHitResult, InHitResult);

UCLASS()
class UNREAL_PORT_CODE_API ACItem_Arrow : public ACItemFiled
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* HitEffect;	// 적이 부딪혔을 때의 이펙트

	UPROPERTY(EditDefaultsOnly)
		FTransform HitEffectTransform;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

public:
	ACItem_Arrow();

	FORCEINLINE void SetEffect(class UParticleSystem* effect) { HitEffect = effect; }
	FORCEINLINE void SetEffectTransform(const FTransform& transfrom) { HitEffectTransform = transfrom; }
	FORCEINLINE void SetArrowType(EArrowType type) { Type = type; }

	FORCEINLINE bool GetStucked() { return bStucked; }

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UPROPERTY(BlueprintAssignable)
		FThrowBeginOverlap OnThrowBeginOverlap;

private:
	void Stucked();
	bool SphereTrace(const FVector& start, const FVector& end);

private:
	EArrowType Type;

	bool bStucked;

};

/*
	1. 화살이 소환된 위치에서 바로 떨어짐
	-	StaticMesh->SetCollisionProfileName("NoCollision"); 로 해결
	-> 이제 화살이 어딘가에 부딪히면 bStucked를 true로 하며,
	   SphereCollision을 Nocollision으로

	   (현재 해결은 함)

	2. 화살의 count가 0이 됨

	3. 화살의 name widget이 호출되지 않음
*/