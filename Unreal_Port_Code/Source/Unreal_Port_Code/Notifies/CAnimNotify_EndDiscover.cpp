#include "CAnimNotify_EndDiscover.h"
#include"Global.h"
#include "GameFramework/Character.h"
#include "Interfaces/ICharacter.h"

FString UCAnimNotify_EndDiscover::GetNotifyName_Implementation() const
{
	return "End_Discover";
}

void UCAnimNotify_EndDiscover::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->End_Discover();
}
