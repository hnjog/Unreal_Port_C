#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Dodge, Climing, Equip, Action, Special, Hitted, Dead, Taunt,
	//For Battle,
	Stiff, KnockOut, WakeUp,
	Rush,AirComboWait, AirCombo,
	//For AI
	Discover, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PORT_CODE_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE EStateType GetType() { return Type; }

public:
	UCStateComponent();

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDodgeMode() { return Type == EStateType::Dodge; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsClimingMode() { return Type == EStateType::Climing; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsSpecialMode() { return Type == EStateType::Special; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTauntMode() { return Type == EStateType::Taunt; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsStiffMode() { return Type == EStateType::Stiff; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsKnockOutMode() { return Type == EStateType::KnockOut; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWakeUpMode() { return Type == EStateType::WakeUp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsRushMode() { return Type == EStateType::Rush; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAirComboWaitMode() { return Type == EStateType::AirComboWait; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAirComboMode() { return Type == EStateType::AirCombo; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDiscoverMode() { return Type == EStateType::Discover; }

public:
	UFUNCTION(BlueprintCallable)
		void SetIdleMode();
	void SetDodgeMode();
	void SetClimingMode();
	void SetEquipMode();
	void SetActionMode();
	void SetSpecialMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetTauntMode();
	void SetStiffMode();
	void SetKnockOutMode();
	void SetWakeUpMode();
	UFUNCTION(BlueprintCallable)
		void SetRushMode();

	void SetAirComboWaitMode();
	UFUNCTION(BlueprintCallable)
		void SetAirComboMode();
	void SetDiscoverMode();

private:
	void ChangeType(EStateType InNewType);

public:
	// character state Change => Call Events
	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;

protected:
	virtual void BeginPlay() override;

private:
	EStateType Type;

};
