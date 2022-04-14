#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FThrowOverlap, FHitResult, InHitResult, bool, Charge);

UCLASS()
class UNREAL_PORT_CODE_API ACArrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Trail;

public:	
	ACArrow();

	FORCEINLINE void SetCharged() { bCharged = true; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDestoryEvent(AActor* DestroyedActor);

public:
	UPROPERTY(BlueprintAssignable)
		FThrowOverlap OnThrowOverlap;

private:

	bool bCharged = false;

	class UMaterialInstanceDynamic* EffectMat;
};
