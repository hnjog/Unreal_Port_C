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
		class UParticleSystem* HitEffect;	// ���� �ε����� ���� ����Ʈ

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
	1. ȭ���� ��ȯ�� ��ġ���� �ٷ� ������
	-	StaticMesh->SetCollisionProfileName("NoCollision"); �� �ذ�
	-> ���� ȭ���� ��򰡿� �ε����� bStucked�� true�� �ϸ�,
	   SphereCollision�� Nocollision����

	   (���� �ذ��� ��)

	2. ȭ���� count�� 0�� ��

	3. ȭ���� name widget�� ȣ����� ����
*/