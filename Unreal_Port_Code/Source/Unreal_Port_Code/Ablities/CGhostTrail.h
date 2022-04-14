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
	�����⿡ ����� �ܻ�

	- ���� �����Ͽ� ���
	(player �ʿ���)

	- ��� �� set skeletal�� �̸� �־��־�� ��
	actordeffered
	- �� ����
	final

*/