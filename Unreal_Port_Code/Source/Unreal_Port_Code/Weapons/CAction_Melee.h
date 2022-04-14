#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAction.h"
#include "CAction_Melee.generated.h"

DECLARE_DELEGATE(FAirCombo);

UCLASS()
class UNREAL_PORT_CODE_API ACAction_Melee : public ACAction
{
	GENERATED_BODY()

public:
	// �޺� ���� �ִ� ���� ��
	FORCEINLINE void EnableCombo() { bEnableAttack = true; }
	FORCEINLINE void DisableCombo() { bEnableAttack = false; }

	// Player���� OnSpecial �����̸�, SuccessGuard�� True�� ���������� �Ǵ�
	FORCEINLINE bool IsGuard() { return bOnGuard; }
	FORCEINLINE bool SuccessParry() { return bParrying; }
	FORCEINLINE bool SuccessGuard() { return bSuccessGuard; }

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
		void OnGuardPointBeginOverlap(class AActor* DefenseTo, class AActor* InAttackCauser);
	UFUNCTION()
		void OnGuardPointEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter);

private:
	UFUNCTION()
		void ResetGlobalDilation();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	FAirCombo OnAirCombo;

private:
	void Parrying(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter);
	void Guard();

private:
	int32 IndexAttack;
	bool bEnableAttack;
	bool bExistAttack;
	bool bLastAttack;

	int32 IndexSpecial;
	bool bEnableSpecial;
	bool bExistSpecial;
	bool bLastSpecial;

	int32 IndexAir = 0;
	bool bEnableAirCombo = true;

	//For Guard (������ Melee�� �̰ɷ� ������ �����̹Ƿ�)
	bool bOnGuard = false;
	bool bParrying = false;
	bool bSuccessGuard = false;
	float GuardTimer = 0.0f;

	float HitStopRate = 0.2f;

	// �ٴ���Ʈ ������
	TArray<class ACharacter*> HittedCharacters;
	// ������
	TArray<class ACharacter*> StiffedCharacters;

	class ACSpecialPoint* GuardPoint;

};

/*
	���� ������ ���Է� ��� �ð��� ���� ���, (combo)
	End_DoAction���� �����⿡
	
	Idle State�� ��ȯ��

	Idle ���°� �Ǵ� ���,

	1. AirCombo ���¿� �����Ͽ�����,
	1�� ���� �� ������ �̷�� ���� ����

	2. AirCombo�� ������ Ƚ���� ��� ������

	AirCombo End ���� �Լ��� ����� ȣ���ؾ� ��
	

*/
