#include "CAnimNotify_Dodge.h"
#include"Global.h"
#include "GameFramework/Character.h"
#include "Interfaces/ICharacter.h"

FString UCAnimNotify_Dodge::GetNotifyName_Implementation() const
{
	return "Dodge_End";
}

void UCAnimNotify_Dodge::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->End_Dodge();
}

