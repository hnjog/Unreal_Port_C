#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeapon.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;

};

USTRUCT(BlueprintType)
struct FActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float PowerRate = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;

};

USTRUCT(BlueprintType)
struct FSpecialData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float PowerRate = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;

};

USTRUCT(BlueprintType)
struct FAirComboData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float PowerRate = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;

};

UCLASS()
class UNREAL_PORT_CODE_API UCWeapon : public UObject
{
	GENERATED_BODY()

public:
	friend class UCWeaponData;

public:
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACAction* GetAction() { return Action; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

	FORCEINLINE void SetAttachment(ACAttachment* attach) { Attachment = attach; }

private:
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACAction* Action;

};
