#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
}

//PlayEquipMontage, PawnControl, ChangeBodyMaterial
void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (!!Data.AnimMontage)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);
	else
	{
		// 재생할 몽타주가 없는 녀석들(fist 등)
		Begin_Equip();
		End_Equip();
	}

	if (Data.bPawnControl)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

}

//Socket? Particle? Decal? -> Delegate Execute
void ACEquipment::Begin_Equip_Implementation()
{
	// 뭐가 들어올줄 모르므로, delegate를 사용할 예정
	// delegate -> 기능이 구현되지 않았어도 먼저 작성할 수 있음 (이후 바인딩 해주면 됨)
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

//State -> Idle
void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode();
	bEquipped = true;
}

// OrientRotation -> true
void ACEquipment::Unequip_Implementation()
{
	bEquipped = false;

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();
}
