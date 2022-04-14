#include "CAnimNotifyState_Combo.h"
#include"Global.h"
#include"Components/CActionComponent.h"
#include"Weapons/CWeaponData.h"
#include"Weapons/CAction_Melee.h"

FString  UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return"Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACAction_Melee* melee = Cast<ACAction_Melee>(action->GetCurrentData()->GetAction());
	CheckNull(melee);


	melee->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACAction_Melee* melee = Cast<ACAction_Melee>(action->GetCurrentData()->GetAction());
	CheckNull(melee);

	melee->DisableCombo();
}
