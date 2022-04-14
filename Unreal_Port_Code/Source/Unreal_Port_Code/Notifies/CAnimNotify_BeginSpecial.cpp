#include "CAnimNotify_BeginSpecial.h"
#include "Global.h"
#include"Weapons/CWeaponData.h"
#include"Weapons/CAction.h"
#include"Components/CActionComponent.h"


FString UCAnimNotify_BeginSpecial::GetNotifyName_Implementation() const
{
	return "BeginSpecial";
}

void UCAnimNotify_BeginSpecial::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	//Meshcomp->GetOwner-> ActionComp->GetCurrent->GetDoAction->Begin_DoAction
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrentData()->GetAction()->Begin_Special();


}