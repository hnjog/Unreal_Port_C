#include "CAnimNotifyState_Collision.h"
#include"Global.h"
#include"Components/CActionComponent.h"
#include"Weapons/CWeaponData.h"
#include"Weapons/CAttachment.h"

FString  UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return"Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	action->GetCurrentData()->GetAttachment()->OnCollision();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	action->GetCurrentData()->GetAttachment()->OffCollision();
}
