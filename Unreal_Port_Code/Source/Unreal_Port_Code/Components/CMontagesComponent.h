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
	FORCEINLINE bool IsDataValid(EStateType state) { return (!!Datas[(int32)state]); }

public:	
	UCMontagesComponent();

	void PlayEquip();
	void PlayHitted();
	void PlayStiff();
	void PlayDead();
	void PlayWakeUp();

	// AI
	void PlayTaunt();
	void PlayDiscover();

protected:
	virtual void BeginPlay() override;

private:
	void PlayAnimMontage(EStateType InStateType);

private:
	FMontageData* Datas[(int32)EStateType::Max];

};

/*
	이걸 캐릭터 기반 녀석들에게 넣어주면
	되긴 하는데...


*/