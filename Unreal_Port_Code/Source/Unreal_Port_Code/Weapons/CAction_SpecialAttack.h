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
	//�θ� UFUCION �̸� �ڽĵ� �ش� �ɼ��� ���ϰ� ����� ��
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

	// �ٴ���Ʈ ������
	TArray<class ACharacter*> HittedCharacters;
	// ������
	TArray<class ACharacter*> SpecialHittedCharacters;

	class ACSpecialPoint* SpecialPoint;
};

/*
	�⺻������ Melee �� ���������
	Guard Point �� ����ϴ� ���� �ٸ�

	-> DoSpecial ��
	DoAction ó�� ������ ��

	�ش� ������


*/