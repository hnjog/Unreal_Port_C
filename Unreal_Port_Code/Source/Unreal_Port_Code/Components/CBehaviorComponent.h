#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehavorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Avoid, Defend,
	Taunt, Special, Doubt, Discover,Smite, Dead,
	Stiff,KnockOut, WakeUp, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehavorType, InPrevType, EBehavorType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PORT_CODE_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";	// 분기 태우는 용도로 사용할 예정 (Black board key 값 비교 서비스)

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";

	UPROPERTY(EditAnywhere)
		FName StanceKey = "Stance";

public:
	FORCEINLINE void SetBlackBoard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	class ACPlayer* GetTargetPlayer();

private:
	void ChangeType(EBehavorType InType);
	EBehavorType GetType();

public:
	// 변수를 안만들 예정이라 인라인 안씀
	UFUNCTION(BlueprintCallable) bool IsWaitMode();
	UFUNCTION(BlueprintCallable) bool IsApproachMode();
	UFUNCTION(BlueprintCallable) bool IsActionMode();
	UFUNCTION(BlueprintCallable) bool IsPatrolMode();
	UFUNCTION(BlueprintCallable) bool IsHittedMode();
	UFUNCTION(BlueprintCallable) bool IsAvoidMode();
	UFUNCTION(BlueprintCallable) bool IsDefendMode();
	UFUNCTION(BlueprintCallable) bool IsTauntMode();
	UFUNCTION(BlueprintCallable) bool IsSpecialMode();
	UFUNCTION(BlueprintCallable) bool IsDoubtMode();
	UFUNCTION(BlueprintCallable) bool IsDiscoverMode();
	UFUNCTION(BlueprintCallable) bool IsSmiteMode();
	UFUNCTION(BlueprintCallable) bool IsDeadMode();
	UFUNCTION(BlueprintCallable) bool IsStiffMode();
	UFUNCTION(BlueprintCallable) bool IsKnockOutMode();
	UFUNCTION(BlueprintCallable) bool IsWakeUpMode();

	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDefendMode();
	void SetTauntMode();
	void SetSpecialMode();
	void SetDoubtMode();
	void SetDiscoverMode();
	void SetSmiteMode();
	void SetDeadMode();
	void SetStiffMode();
	void SetKnockOutMode();
	void SetWakeUpMode();

public:
	UCBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

private:
	class UBlackboardComponent* Blackboard;


};
