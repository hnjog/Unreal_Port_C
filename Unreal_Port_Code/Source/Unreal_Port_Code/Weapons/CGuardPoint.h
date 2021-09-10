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
	�̳༮�� ĸ�� ������Ʈ���� ������ ���̱⿡
	�������� ���� ���� �����൵ ��

	�� �༮�� ��ġ?
	�ణ �������� �ؾ� ��

	��Ŭ���� Ÿ���� �������� �ϴ� ������ �ϰ� ����

*/