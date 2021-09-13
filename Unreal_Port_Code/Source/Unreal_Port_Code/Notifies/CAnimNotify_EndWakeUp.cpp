#include "CAnimNotify_EndWakeUp.h"
#include"Global.h"
#include "Interfaces/ICharacter.h"

FString  UCAnimNotify_EndWakeUp::GetNotifyName_Implementation() const
{
	return "WakeUp";
}

void UCAnimNotify_EndWakeUp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->End_WakeUp();
}
