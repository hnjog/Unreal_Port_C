#include "CBehaviorComponent.h"
#include"Global.h"
#include"Characters/CPlayer.h"
#include<BehaviorTree/BlackboardComponent.h>

UCBehaviorComponent::UCBehaviorComponent()
{

}

void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehavorType::Wait;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehavorType::Approach;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehavorType::Action;
}

bool UCBehaviorComponent::IsPatrolMode()
{
	return GetType() == EBehavorType::Patrol;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehavorType::Hitted;
}

bool UCBehaviorComponent::IsAvoidMode()
{
	return GetType() == EBehavorType::Avoid;
}

bool UCBehaviorComponent::IsDefendMode()
{
	return GetType() == EBehavorType::Defend;
}

bool UCBehaviorComponent::IsTauntMode()
{
	return GetType() == EBehavorType::Taunt;
}

bool UCBehaviorComponent::IsSpecialMode()
{
	return GetType() == EBehavorType::Special;
}

bool UCBehaviorComponent::IsDoubtMode()
{
	return GetType() == EBehavorType::Doubt;
}

bool UCBehaviorComponent::IsDiscoverMode()
{
	return GetType() == EBehavorType::Discover;
}

bool UCBehaviorComponent::IsSmiteMode()
{
	return GetType() == EBehavorType::Smite;
}

bool UCBehaviorComponent::IsDeadMode()
{
	return GetType() == EBehavorType::Dead;
}

bool UCBehaviorComponent::IsStiffMode()
{
	return GetType() == EBehavorType::Stiff;
}

bool UCBehaviorComponent::IsKnockOutMode()
{
	return GetType() == EBehavorType::KnockOut;
}

bool UCBehaviorComponent::IsWakeUpMode()
{
	return GetType() == EBehavorType::WakeUp;
}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehavorType::Wait);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehavorType::Approach);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehavorType::Action);
}

void UCBehaviorComponent::SetPatrolMode()
{
	ChangeType(EBehavorType::Patrol);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehavorType::Hitted);
}

void UCBehaviorComponent::SetAvoidMode()
{
	ChangeType(EBehavorType::Avoid);
}

void UCBehaviorComponent::SetDefendMode()
{
	ChangeType(EBehavorType::Defend);
}

void UCBehaviorComponent::SetTauntMode()
{
	ChangeType(EBehavorType::Taunt);
}

void UCBehaviorComponent::SetSpecialMode()
{
	ChangeType(EBehavorType::Special);
}

void UCBehaviorComponent::SetDoubtMode()
{
	ChangeType(EBehavorType::Doubt);
}

void UCBehaviorComponent::SetDiscoverMode()
{
	ChangeType(EBehavorType::Discover);
}

void UCBehaviorComponent::SetSmiteMode()
{
	ChangeType(EBehavorType::Smite);
}

void UCBehaviorComponent::SetDeadMode()
{
	ChangeType(EBehavorType::Dead);
}

void UCBehaviorComponent::SetStiffMode()
{
	ChangeType(EBehavorType::Stiff);
}

void UCBehaviorComponent::SetKnockOutMode()
{
	ChangeType(EBehavorType::KnockOut);
}

void UCBehaviorComponent::SetWakeUpMode()
{
	ChangeType(EBehavorType::WakeUp);
}

ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

void UCBehaviorComponent::ChangeType(EBehavorType InType)
{
	EBehavorType prev = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(prev, InType);
}

EBehavorType UCBehaviorComponent::GetType()
{
	return (EBehavorType)Blackboard->GetValueAsEnum(BehaviorKey);
}
