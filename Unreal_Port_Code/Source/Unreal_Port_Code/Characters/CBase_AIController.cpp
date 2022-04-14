#include "CBase_AIController.h"
#include"Global.h"
#include<BehaviorTree/BlackboardComponent.h>
#include<BehaviorTree/BehaviorTree.h>
#include<Perception/AIPerceptionComponent.h>
#include<Perception/AISenseConfig_Sight.h>
#include"Components/CBehaviorComponent.h"
#include"CPlayer.h"

ACBase_AIController::ACBase_AIController()
{
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 400.0f;
	Sight->LoseSightRadius = 600.0f;
	Sight->PeripheralVisionAngleDegrees = 90.0f;
	Sight->SetMaxAge(2.0f);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;	// 아군 감지 x
	Sight->DetectionByAffiliation.bDetectNeutrals = false;		// 중립 감지 x

	Perception->ConfigureSense(*Sight);
}

float ACBase_AIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACBase_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACBase_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(bDrawDebug);

	FVector center = OwnerEnemy->GetActorLocation();
	center.Z -= DebugHeight;
	DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
	DrawDebugCircle(GetWorld(), center, BehaviorRange, 300, FColor::Red, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);

}

void ACBase_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);

	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	SetGenericTeamId(OwnerEnemy->GetTeamID());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACBase_AIController::OnPerceptionUpdated);

	Behavior->SetBlackBoard(Blackboard);

	OwnerEnemy->OnStanceTypeChanged.AddDynamic(this, &ACBase_AIController::OnStanceChanged);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}

void ACBase_AIController::OnUnPossess()
{
	Super::OnUnPossess();
	Perception->OnPerceptionUpdated.Clear();	// 델리게이트 날리기

}

void ACBase_AIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	ACPlayer* player = nullptr;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);
		if (!!player)
		{
			break;
		}
	}

	Blackboard->SetValueAsObject("Player", player);
	CheckTrue(!!player);
	ClearFocus(EAIFocusPriority::Gameplay);
}

float ACBase_AIController::GetSightMaxAge()
{
	return Sight->GetMaxAge();
}

void ACBase_AIController::OnStanceChanged(EEnemyStance InPrevStance, EEnemyStance InNewStance)
{
	StanceType = InNewStance;
	Blackboard->SetValueAsEnum("Stance", (int32)StanceType);
}
