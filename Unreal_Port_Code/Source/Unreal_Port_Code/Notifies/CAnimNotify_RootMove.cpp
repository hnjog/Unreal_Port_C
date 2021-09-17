#include "CAnimNotify_RootMove.h"
#include"Global.h"
#include "Interfaces/ICharacter.h"

FString  UCAnimNotify_RootMove::GetNotifyName_Implementation() const
{
	return "Root_Move";
}

void UCAnimNotify_RootMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->RootMove(Animation->GetName());
}
