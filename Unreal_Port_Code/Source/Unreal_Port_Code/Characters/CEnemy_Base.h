#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/CStateComponent.h"
#include "Interfaces/ICharacter.h"
#include "Interfaces/ITargetAble.h"
#include "CEnemy_Base.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACEnemy_Base : public ACharacter, public IICharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		float LauchValue = 100.0f;

	UPROPERTY(EditDefaultsOnly)
		float LauchUpValue = 30.0f;

	UPROPERTY(EditDefaultsOnly)
		float WakeUpTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
		float LerpSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly)
		float SpecialIntensity = 5.0f;

	UPROPERTY(EditDefaultsOnly)
		float HealthUIOnTime = 5.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

protected:
	UPROPERTY(EditDefaultsOnly)
		class USceneComponent* TargetPoint;

protected: //ActorCompnent
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "widgets")
		TSubclassOf<class UCUserWidget_HealthBar> HealthWidgetClass;

	UPROPERTY(BlueprintReadOnly)
		class UWidgetComponent* HealthWidget;

public:
	// Sets default values for this character's properties
	ACEnemy_Base();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void AboryByDamage();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DamageUpdate(float amount, bool bDamage = true);

	UFUNCTION()
		void OffHealthUI();

protected:
	// For Material Param
	virtual void Idle();
	virtual void Special();

	virtual void Hitted();
	virtual void End_Hit() override;

	virtual void Dead();
	virtual void End_Dead() override;

	virtual void Taunt();
	virtual void End_Taunt() override;

	virtual void DiscoverTarget();
	virtual void End_Discover() override;

	virtual void Stiff();
	virtual void End_Stiff() override;

	virtual void WakeUp();
	virtual void End_WakeUp() override;

protected:
	virtual void LaunchByHitted() override;
	virtual void Popcorn() override;
	virtual void End_Popcorn() override;

private:
	void FixMesh();
	void LerpTurn(float Delta);

private:
	class AController* DamageInstigator;

protected:
	class UMaterialInstanceDynamic* BodyMaterial;

	FTimeline SpecialTimeline;
	class UCurveFloat* Curve;

protected:
	bool bDamagedLastAttack;
	FHitResult HittedResult;

	float WakeUpTimer;

	bool bUseLerpTurn = false;

	TArray<class ACTargetPoint*> Targets;
	TSubclassOf<ACTargetPoint> TargetClass;

	UINT ComboCount = 0;
};
