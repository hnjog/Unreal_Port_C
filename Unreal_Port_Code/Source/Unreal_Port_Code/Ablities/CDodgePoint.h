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
	player���� ȸ�� ���۽�

	����

	�ش� �ڸ��� ���� ª�� �ð� �����ϸ�,
	�ε��� ���
	
	Player�� �����ϵ���
	���������

	bind �� �����,
	destroy ���� unbind ���Ѿ� ��

*/