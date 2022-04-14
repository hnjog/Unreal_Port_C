#include "CAnimNotify_CreateGhostTrail.h"
#include"Global.h"
#include "Interfaces/ICharacter.h"

FString  UCAnimNotify_CreateGhostTrail::GetNotifyName_Implementation() const
{
	return "GhostTrail";
}

void UCAnimNotify_CreateGhostTrail::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->CreateGhostTrail();
}
