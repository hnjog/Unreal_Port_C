#include "CAnimNotify_EndSpecial.h"
#include "Global.h"
#include"Weapons/CWeaponData.h"
#include"Weapons/CAction.h"
#include"Components/CActionComponent.h"

FString UCAnimNotify_EndSpecial::GetNotifyName_Implementation() const
{
	return "End_Special";
}

void UCAnimNotify_EndSpecial::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrentData()->GetAction()->End_Special();
}