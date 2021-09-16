#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAction.h"
#include "CAction_SpecialAttack.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACAction_SpecialAttack : public ACAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnableAttack = true; }
	FORCEINLINE void DisableCombo() { bEnableAttack = false; }

public:
	void DoAction() override;
	void CancelSpecial() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	// CancelSpecial

	void DoSpecial() override;
	virtual void Begin_Special() override;
	virtual void End_Special() override;

public:
	//부모가 UFUCION 이면 자식도 해당 옵션을 지니고 상속이 됨
	// for Attack
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

	UFUNCTION()
		void OnSpecialPointBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter);
	UFUNCTION()
		void OnSpecialPointEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter);

private:
	UFUNCTION()
		void ResetGlobalDilation();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


private:
	int32 IndexAttack;
	bool bEnableAttack;
	bool bExistAttack;
	bool bLastAttack;

	int32 IndexSpecial;
	bool bEnableSpecial;
	bool bExistSpecial;
	bool bLastSpecial;

	// 다단히트 방지용
	TArray<class ACharacter*> HittedCharacters;
	// 경직용
	TArray<class ACharacter*> SpecialHittedCharacters;

	class ACSpecialPoint* SpecialPoint;
};

/*
	기본적으로 Melee 랑 비슷하지만
	Guard Point 를 사용하는 것이 다름

	-> DoSpecial 이
	DoAction 처럼 공격을 함

	해당 공격은


*/