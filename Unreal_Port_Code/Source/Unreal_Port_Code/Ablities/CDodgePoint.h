#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDodgePoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDodgePointBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_DELEGATE(FSuccessDodge);

UCLASS()
class UNREAL_PORT_CODE_API ACDodgePoint : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

public:	
	ACDodgePoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
		FSuccessDodge OnSuccessDodge;

	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool IsSpawned = false;
	float OutTimer;
	float OutTime = 0.5f;

};

/*
	player에서 회피 시작시

	생성

	해당 자리에 아주 짧은 시간 지속하며,
	부딪힌 경우
	
	Player에 전달하도록
	사라지도록

	bind 한 경우라면,
	destroy 전에 unbind 시켜야 함

*/