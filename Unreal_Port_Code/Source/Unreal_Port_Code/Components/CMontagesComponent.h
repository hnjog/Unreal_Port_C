#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CStateComponent.h"
#include "CMontagesComponent.generated.h"

USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PORT_CODE_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;

public:
	bool IsDataValid(EStateType state);

public:	
	UCMontagesComponent();

	void PlayEquip();
	void PlayHitted();
	void PlayStiff();
	void PlayDead();
	void PlayWakeUp();
	void PlayDodge();

	// AI
	void PlayTaunt();
	void PlayDiscover();

protected:
	virtual void BeginPlay() override;

private:
	void PlayAnimMontage(EStateType InStateType, int32 index = 0);

	float Calculate(const FVector& Velocity, const FRotator& BaseRotation);

private:
	FMontageData* Datas[(int32)EStateType::Max][10];

};
