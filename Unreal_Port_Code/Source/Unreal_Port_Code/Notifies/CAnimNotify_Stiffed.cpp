#include "CAnimNotify_Stiffed.h"
#include"Global.h"
#include "Interfaces/ICharacter.h"

FString  UCAnimNotify_Stiffed::GetNotifyName_Implementation() const
{
	return "Stiffed";
}

void UCAnimNotify_Stiffed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->End_Stiff();
}
