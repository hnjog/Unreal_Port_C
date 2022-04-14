#include "CGhostTrail.h"
#include"Global.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include<Materials/MaterialInstanceDynamic.h>

ACGhostTrail::ACGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &PoseableMesh, "PoseableMesh");
	
	CHelpers::GetAsset(&GhostMaterial, "MaterialInstanceConstant'/Game/Material/Effect/MI_Shadow.MI_Shadow'");
}

void ACGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSpawned)
	{
		FadeCountDown -= DeltaTime;
		for (int i = 0; i < Materials.Num(); i++)
		{
			Materials[i]->SetScalarParameterValue("Opacity", FadeCountDown / FadeOutTime);
		}
		if (FadeCountDown < 0)
		{
			Destroy();
		}
	}
}

void ACGhostTrail::Init(USkeletalMeshComponent* Pawn)
{
	if (!!Mesh)
		PoseableMesh->SetSkeletalMesh(Mesh);

	PoseableMesh->CopyPoseFromSkeletalComponent(Pawn);
	TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();

	for (int i = 0; i < Mats.Num(); i++)
	{
		Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial));
		PoseableMesh->SetMaterial(i, Materials[i]);
	}
	FadeCountDown = FadeOutTime;
	IsSpawned = true;
}

