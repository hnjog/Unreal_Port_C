#include "CAnimNotify_EndTaunt.h"
#include"Global.h"
#include "GameFramework/Character.h"
#include "Interfaces/ICharacter.h"

FString UCAnimNotify_EndTaunt::GetNotifyName_Implementation() const
{
	return "End_Taunt";
}

void UCAnimNotify_EndTaunt::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->End_Taunt();
}
