#include "CAnimInstance.h"
#include"Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	
	if (!!action)
	{
		action->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
	}

	if (!!state)
	{
		state->OnStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnStateTypeChanged);
	}

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());
	Pitch = character->GetBaseAimRotation().Pitch;
	//Falling 

	// state 가 KnockOut 상태이면, Speed와 Direction을 사용하지 않도록 해야 할듯
	if (character->GetCharacterMovement()->IsFalling() == true)
	{
		Speed = 0.0f;
		Direction = 0.0f;
		Pitch = 0.0f;
	}

	if (ActionType == EActionType::Bow && StateType == EStateType::Special)
	{
		Aim = true;
	}
	else
	{
		Aim = false;
	}

	UCharacterMovementComponent* charMove = Cast<UCharacterMovementComponent>(TryGetPawnOwner()->GetMovementComponent());
	CheckNull(charMove);

	Falling = charMove->IsFalling();
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}

void UCAnimInstance::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	StateType = InNewType;
}