#include "CAnimInstance_Enemy.h"
#include"Global.h"
#include "GameFramework/Character.h"
#include"CBase_AIController.h"

void UCAnimInstance_Enemy::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	/*ACBase_AIController* controller = Cast<ACBase_AIController>(TryGetPawnOwner()->GetController());
	CheckNull(controller);

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);*/

	ACEnemy_AI* ownerEnemy = Cast<ACEnemy_AI>(TryGetPawnOwner());
	CheckNull(ownerEnemy);

	ownerEnemy->OnStanceTypeChanged.AddDynamic(this, &UCAnimInstance_Enemy::OnStanceChanged);
}

void UCAnimInstance_Enemy::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UCAnimInstance_Enemy::OnStanceChanged(EEnemyStance InPrevStance, EEnemyStance InNewStance)
{
	StanceType = InNewStance;
}
