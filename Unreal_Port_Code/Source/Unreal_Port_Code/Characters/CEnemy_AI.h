#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_Base.h"
#include "CEnemy_AI.generated.h"

UENUM(BlueprintType)
enum class EEnemyStance : uint8
{
	Idle, Battle, Doubt,Hitted, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStanceTypeChanged, EEnemyStance, InPrevStance, EEnemyStance, InNewStance);

UCLASS()
class UNREAL_PORT_CODE_API ACEnemy_AI : public ACEnemy_Base
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;


public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	FORCEINLINE EEnemyStance GetStanceType() { return Stance; }

public:
	ACEnemy_AI();

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleStance() { return Stance == EEnemyStance::Idle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBattleStance() { return Stance == EEnemyStance::Battle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDoubtStance() { return Stance == EEnemyStance::Doubt; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedStance() { return Stance == EEnemyStance::Hitted; }

public:
	void SetIdleStance();
	void SetBattleStance();
	void SetDoubtStance();
	void SetHittedStance();

protected:
	virtual void BeginPlay() override;

	virtual void End_Dead() override;

public:
	UPROPERTY(BlueprintAssignable)
		FStanceTypeChanged OnStanceTypeChanged;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void RootMove() override;

private:
	EEnemyStance Stance;

private:
	void ChangeStanceType(EEnemyStance InNewType);

};

/*
	Stance
	- �Ϲ� ����
	- ��Ʋ ����
	- �ǽ� ����

	-> Anim �ʿ��� ����� �༮����
	-> delegate�� bind ��������


*/