// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

private:
	// wizet ����� ���⿡

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
	virtual void Hitted();
	virtual void End_Hit() override;

	virtual void Dead();
	virtual void End_Dead() override;

	virtual void Taunt();
	virtual void End_Taunt() override;

	virtual void Stiff();
	virtual void End_Stiff() override;

	virtual void DiscoverTarget();
	virtual void End_Discover() override;

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
	bool bDamagedLastAttack;
	FHitResult HittedResult;

	float WakeUpTimer;

	bool bUseLerpTurn = false;

};

/*

	2. �ǰ� �̺�Ʈ (TakeDamage)��
	Stiff, Down ���� �����
	(Stiff�� CMontage�� �߰��ϱ�)

	3. player�� enemy��
	����
	OnStiff()
	OnDown() �Լ� �߰�
	(�ַ� ���� �ð� ������ Idle �� ���ƿ��� ��)
	(���� ������� ����)

	4. ������ Ÿ��
	���� Ȯ���ϴ� ��?
	UDamageType

	5. enemy ������ ���� �����

	state��
	Down �� Airbone�� ���ļ�
	KnockOut ���·� �ٲٰ�,
	�Ͼ�� WakeUp ���·� �ٽ� ����� �������ƺ���

	-> KnockOut �����̸�,
	(�̶� PopCorn ���¸� �����Ŵ)
	���� ��� �ִ� ���º���
	Timer�� �����̸�,
	Timer�� WakeUpTimer��
	�Ѿ�� �ʱ�ȭ ����,
	State�� WakeUp ���·�,

	(�ǰݵ� ��, ���� ���¿� ����, ������� �ʴ°� ���ƺ��� ���� �����Ѵٰ� ������)

	(�� ��, Skeletal �̶�, Popcorn�� ����ϴ��� �����İ� ������ ���������� ���� �� �ѵ�...)

	(�ϴ� ��� ���������� �װ� �� ���� Popcorn ���ѵ� �ɵ�...?)

	popcorn ��ü�� ������??
	-> Mesh�� root �� ���� ��� ��
	-> �����ؾ� �ϳ�?

	���� ���ϴ� �ðܳ����� ���� ����� ��
	'popcorn' �̿��� �ұ�?

	IK �� ���ε� �ʿ��ϱ� ��

	+ KnockOut ���� �ٽ�
	Down �� Airbonre ���·� ����°� ���ƺ��̱� ��
	(��� ���, Skeltal Mesh�� Physics �ý��� �� �̿��ϸ� �ذ��� �� ��������?)

	(�ϴ� ����Բ�,
	���ư� ��, Popcorn ���·� �����,
	actor ��ġ��
	popcorn ���·� ���鶧,
	actor�� ��ġ�� �����ϰ�,

	�޽��� Physics ����� �̿�?

	-> �ϴ� Enemy�� Down, Airborne WakeUp ���¿� ������ �� �ִ��� ã�ƺ���,
	�״��� �ٽ� Enemy ������ ����

	(�÷��̾� �ʿ��� �����ϰ�
	-> ���� �ִϸ��̼ǵ� ã�ƾ� ��)

	-> ���� ���¸� �����ϰ�

	player guard ��� ����


*/