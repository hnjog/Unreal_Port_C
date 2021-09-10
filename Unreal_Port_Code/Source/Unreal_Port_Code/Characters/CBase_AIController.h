#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include"CEnemy_AI.h"
#include "CBase_AIController.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACBase_AIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.0f;	// Change By Purpose

	UPROPERTY(EditAnywhere)
		float AttackRange = 300.0f;		// For By Pattern

	UPROPERTY(EditAnywhere)
		float SpecialRange = 150.0f;	// For By Pattern

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;

	UPROPERTY(EditAnywhere)
		float DebugHeight = 50.0f;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;


public:
	FORCEINLINE float GetBehaviorRange() const { return BehaviorRange; }
	float GetSightMaxAge();

	//FORCEINLINE class UCBehaviorComponent* GetBehaviorComp() { return Behavior;}

public:
	ACBase_AIController();

	float GetSightRadius();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	UFUNCTION()
		void OnStanceChanged(EEnemyStance InPrevStance, EEnemyStance InNewStance);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EEnemyStance StanceType;

private:
	class ACEnemy_AI* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;
};
