#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetDodgeMode()
{
	ChangeType(EStateType::Dodge);
}

void UCStateComponent::SetClimingMode()
{
	ChangeType(EStateType::Climing);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetSpecialMode()
{
	ChangeType(EStateType::Special);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetTauntMode()
{
	ChangeType(EStateType::Taunt);
}

void UCStateComponent::SetStiffMode()
{
	ChangeType(EStateType::Stiff);
}

void UCStateComponent::SetKnockOutMode()
{
	ChangeType(EStateType::KnockOut);
}

void UCStateComponent::SetWakeUpMode()
{
	ChangeType(EStateType::WakeUp);
}

void UCStateComponent::SetRushMode()
{
	ChangeType(EStateType::Rush);
}

void UCStateComponent::SetAirComboWaitMode()
{
	ChangeType(EStateType::AirComboWait);
}

void UCStateComponent::SetAirComboMode()
{
	ChangeType(EStateType::AirCombo);
}

void UCStateComponent::SetDiscoverMode()
{
	ChangeType(EStateType::Discover);
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prev = Type;
	Type = InNewType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prev, InNewType);
}