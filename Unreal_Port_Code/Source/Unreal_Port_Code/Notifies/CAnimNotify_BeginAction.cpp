#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include"Weapons/CWeaponData.h"
#include"Weapons/CAction.h"
#include"Components/CActionComponent.h"


FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	//Meshcomp->GetOwner-> ActionComp->GetCurrent->GetDoAction->Begin_DoAction
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrentData()->GetAction()->Begin_DoAction();


}