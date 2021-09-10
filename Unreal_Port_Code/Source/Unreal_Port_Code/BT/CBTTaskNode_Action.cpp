#include "CBTTaskNode_Action.h"
#include"Global.h"
#include<Characters/CBase_AIController.h>
#include<Characters/CEnemy_AI.h>
#include<Characters/CPlayer.h>
#include"Components/CActionComponent.h"
#include"Components/CStateComponent.h"
#include"Components/CBehaviorComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	// TickTask를 사용하기 위하여
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(aiPawn);
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	CheckNullResult(behavior->GetTargetPlayer(), EBTNodeResult::Failed);
	//controller->SetFocus(behavior->GetTargetPlayer());
	controller->StopMovement();

	RunningTime = 0.0f;
	action->DoAction();

	return EBTNodeResult::InProgress;
	//InProgress
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	RunningTime += DeltaSeconds;
	if (state->IsIdleMode() && RunningTime > AttackDelay)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());

	controller->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Failed;
}

