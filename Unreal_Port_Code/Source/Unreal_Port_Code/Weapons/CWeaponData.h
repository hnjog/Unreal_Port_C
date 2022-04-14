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
	// �ش� Ŭ������ ��ӹ޾ƾ� ������� �� ����

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
	������ ���� Ÿ���� ���� ����� ����?
	-> �̷��� ������ ������ ������� ���� �����͵��� ��ü
*/
