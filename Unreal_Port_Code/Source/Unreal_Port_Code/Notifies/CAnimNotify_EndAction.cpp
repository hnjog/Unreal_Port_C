#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include"Weapons/CWeaponData.h"
#include"Weapons/CAction.h"
#include"Components/CActionComponent.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	//Meshcomp->GetOwner-> ActionComp->GetCurrent->GetDoAction->End_DoAction
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrentData()->GetAction()->End_DoAction();
}