#include "CBTTaskNode_Taunt.h"
#include"Global.h"
#include<Characters/CBase_AIController.h>
#include<Characters/CEnemy_AI.h>
#include<Characters/CPlayer.h>
#include"Components/CStateComponent.h"

UCBTTaskNode_Taunt::UCBTTaskNode_Taunt()
{
	NodeName = "Taunt";

	// TickTask를 사용하기 위하여
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Taunt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	controller->StopMovement();
	controller->ClearFocus(EAIFocusPriority::Gameplay);

	RunningTime = 0.0f;

	state->SetTauntMode();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Taunt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACBase_AIController* controller = Cast<ACBase_AIController>(OwnerComp.GetOwner());
	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	RunningTime += DeltaSeconds;
	if (state->IsIdleMode() && RunningTime > Delay)
	{
		aiPawn->SetIdleStance();
		//controller->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}