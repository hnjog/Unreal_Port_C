#include "CBTTaskNode_Discover.h"
#include"Global.h"
#include<Characters/CBase_AIController.h>
#include<Characters/CEnemy_AI.h>
#include<Characters/CPlayer.h>
#include"Components/CStateComponent.h"
#include"Components/CBehaviorComponent.h"

UCBTTaskNode_Discover::UCBTTaskNode_Discover()
{
	NodeName = "Discover";

	// TickTask�� ����ϱ� ���Ͽ�
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Discover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	CheckNullResult(behavior->GetTargetPlayer(), EBTNodeResult::Failed);
	controller->SetFocus(behavior->GetTargetPlayer());
	controller->StopMovement();

	RunningTime = 0.0f;

	state->SetDiscoverMode();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Discover::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	RunningTime += DeltaSeconds;
	if (state->IsIdleMode() && RunningTime > Delay)
	{
		aiPawn->SetIdleStance();
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
//
//EBTNodeResult::Type UCBTTaskNode_Discover::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	Super::AbortTask(OwnerComp, NodeMemory);
//
//	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
//	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
//
//	//UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);
//
//	
//
//	controller->ClearFocus(EAIFocusPriority::Gameplay);
//
//	RunningTime = 0.0f;
//
//	return EBTNodeResult::Succeeded;
//}
