#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ITargetAble.h"
#include "CTargetPoint.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACTargetPoint : public AActor, public IITargetAble
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
		class USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* TargetWidget;

	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	class UCharacterMovementComponent* CharacterMovement;
	class USkeletalMeshComponent* Mesh;

public:
	ACTargetPoint();

protected:
	virtual void BeginPlay() override;

public:
	//virtual void Tick(float DeltaTime) override;

	virtual void TargetWidgetVisible(bool val) override;
	virtual bool IsTargetStiffed() override;
	virtual bool IsTargetKnockOuted() override;
	virtual bool IsTargetFalling() override;
	virtual FVector TargetLocation() override;

	virtual void TargetBeaten() override;
	virtual void End_TargetBeaten() override;
};

/*
	일단 Attach 시켜야 함

	이걸 블프로 만든 뒤,
	그걸 enemy에서 생성 시키면 됨


*/
