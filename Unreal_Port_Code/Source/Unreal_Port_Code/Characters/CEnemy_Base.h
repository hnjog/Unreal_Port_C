// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/CStateComponent.h"
#include "Interfaces/ICharacter.h"
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

private:
	// wizet 만들면 여기에

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

protected: //ActorCompnent
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

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

};

/*

	5. enemy 강공격 조건 만들기

	state의
	Down 과 Airbone을 합쳐서
	KnockOut 상태로 바꾸고,
	일어나는 WakeUp 상태로 다시 만드는 것이좋아보임

	-> KnockOut 상태이며,
	(이때 PopCorn 상태를 적용시킴)
	땅에 닿아 있는 상태부터
	Timer를 움직이며,
	Timer가 WakeUpTimer를
	넘어서면 초기화 이후,
	State를 WakeUp 상태로,

	(피격될 때, 현재 상태에 따라, 재생하지 않는게 좋아보일 때도 존재한다고 생각함)

	(적 중, Skeletal 이라도, Popcorn을 재생하느냐 마느냐가 좋을지 안좋을지가 있을 듯 한데...)

	(일단 적어도 강공격으로 죽게 된 경우는 Popcorn 시켜도 될듯...?)


*/