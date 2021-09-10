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
		// ����� ��Ÿ�ְ� ���� �༮��(fist ��)
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
	// ���� ������ �𸣹Ƿ�, delegate�� ����� ����
	// delegate -> ����� �������� �ʾҾ ���� �ۼ��� �� ���� (���� ���ε� ���ָ� ��)
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
