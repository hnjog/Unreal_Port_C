#include "CAction.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include"Components/CStateComponent.h"
#include"Components/CStatusComponent.h"

ACAction::ACAction()
	:EquipValue(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACAction::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
}

// Called every frame
void ACAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAction::SetEquipValue(float equipValue)
{
	EquipValue = equipValue;
}
