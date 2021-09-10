#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGuardPoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGuardPointBeginOverlap, class AActor*, DefenseTo, class AActor*, InAttackCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGuardPointEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class UNREAL_PORT_CODE_API ACGuardPoint : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

public:
	ACGuardPoint();

	void OnCollision();
	void OffCollision();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FGuardPointBeginOverlap OnGuardPointBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FGuardPointEndOverlap OnGuardPointEndOverlap;

private:
	class ACharacter* OwnerCharacter;

};

/*
	이녀석은 캡슐 컴포넌트에서 가져올 것이기에
	블프에서 딱히 오픈 안해줘도 됨

	이 녀석의 위치?
	약간 전방으로 해야 함

	우클릭시 타깃이 잡혔으면 하는 생각도 하고 있음

*/