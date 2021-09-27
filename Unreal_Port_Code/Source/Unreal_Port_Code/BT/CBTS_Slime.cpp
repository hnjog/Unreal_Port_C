#include "CBTS_Slime.h"
#include"Global.h"
#include"Characters/CBase_AIController.h"
#include"Characters/CEnemy_AI.h"
#include"Characters/CPlayer.h"
#include"Components/CBehaviorComponent.h"
#include"Components/CStateComponent.h"

UCBTS_Slime::UCBTS_Slime()
{
	NodeName = "Slime";
}

void UCBTS_Slime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	if (state->IsDeadMode())
	{
		behavior->SetDeadMode();
		aiPawn->SetHittedStance();
		return;
	}

	SpecialTimer += DeltaSeconds;

	if (state->IsKnockOutMode())
	{
		behavior->SetKnockOutMode();
		aiPawn->SetHittedStance();
		return;
	}

	if (state->IsStiffMode())
	{
		behavior->SetStiffMode();
		aiPawn->SetHittedStance();
		return;
	}
	// Hitted
	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();
		aiPawn->SetHittedStance();
		return;
	}

	if (state->IsWakeUpMode())
	{
		behavior->SetWakeUpMode();
		aiPawn->SetHittedStance();
		return;
	}
	
	ACPlayer* target = behavior->GetTargetPlayer();

	if (target == nullptr)
	{
		aiPawn->SetIdleStance();
		bDiscover = false;

		if (bTaunt == true && SearchTime <= controller->GetSightMaxAge())
		{
			SearchTime += DeltaSeconds;
			behavior->SetDoubtMode();
			aiPawn->SetDoubtStance();
			return;
		}
		else if(bTaunt == true)
		{
			SearchTime = 0.0f;
			bTaunt = false;
			behavior->SetTauntMode();
			return;
		}
		
		behavior->SetWaitMode();
		return;
	}
	// Target Valid
	else
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();
			return;
		}

		//if (behavior->IsDoubtMode() == true && state->IsDiscoverMode() == false)
		if (bDiscover == false && behavior->IsDiscoverMode() == false)
		{
			behavior->SetDiscoverMode();
			SearchTime = 0.0f;
			bDiscover = true;
			return;
		}

	}

	float distance = aiPawn->GetDistanceTo(target);
	bTaunt = true;
	aiPawn->SetBattleStance();

	// 전투 상태

	/*
		Special 의 조건

		슬라임
		: 튀어오른뒤 대상 지점으로 찍는 방식

		처음 우선 순위
		쿨타임??

		스페셜 우선 검사
		-> 이후 쿨타임이 도는 중이라면,
		액션 사용


	*/

	if (distance < controller->GetSpecialRange() && SpecialTimer >= 10.0f)
	{
		behavior->SetSpecialMode();
		SpecialTimer = 0.0f;
		return;
	}



	// In Act Range
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetActionMode();
		return;
	}

	// In Sight Radius
	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}
