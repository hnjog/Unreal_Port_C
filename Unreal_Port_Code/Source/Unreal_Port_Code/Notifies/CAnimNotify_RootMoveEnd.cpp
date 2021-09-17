#include "CAnimNotify_RootMoveEnd.h"
#include"Global.h"
#include "Interfaces/ICharacter.h"

FString  UCAnimNotify_RootMoveEnd::GetNotifyName_Implementation() const
{
	return "Root_End";
}

void UCAnimNotify_RootMoveEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->RootMoveEnd();
}

