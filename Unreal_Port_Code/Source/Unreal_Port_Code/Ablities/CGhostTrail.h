#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACGhostTrail : public AActor
{
	GENERATED_BODY()

public:
	ACGhostTrail();

public:
	virtual void Tick(float DeltaTime) override;

public:
	void Init(class USkeletalMeshComponent* Pawn);

	FORCEINLINE void SetSkeletal(class USkeletalMesh* mesh) { Mesh = mesh; }

private:
	class UPoseableMeshComponent* PoseableMesh;
	class UMaterialInstance* GhostMaterial;
	class USkeletalMesh* Mesh;
	TArray<class UMaterialInstanceDynamic*> Materials;

	bool IsSpawned = false;
	float FadeCountDown;
	float FadeOutTime = 0.5f;

};

/*
	구르기에 사용할 잔상

	- 액터 생성하여 사용
	(player 쪽에서)

	- 사용 전 set skeletal로 미리 넣어주어야 함
	actordeffered
	- 이 사이
	final

*/