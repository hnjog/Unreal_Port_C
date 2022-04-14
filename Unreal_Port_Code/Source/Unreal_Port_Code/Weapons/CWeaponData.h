#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"CWeapon.h"
#include "CWeaponData.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API UCWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class UCWeapon** OutWeapon);

private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;
	// 해당 클래스를 상속받아야 집어넣을 수 있음

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAction> ActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FActionData> DoActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FSpecialData> DoSpecialDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FAirComboData> AirComboDatas;

	//FAirComboData
};

/*
	데이터 에셋 타입을 새로 만드는 것은?
	-> 이러한 데이터 에셋을 기반으로 기존 데이터들을 대체
*/
