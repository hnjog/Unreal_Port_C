#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAction.h"
#include "CAction_Melee.generated.h"

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

	//For Guard (������ Melee�� �̰ɷ� ������ �����̹Ƿ�)
	bool bOnGuard = false;
	bool bParrying = false;
	bool bSuccessGuard = false;
	float GuardTimer = 0.0f;

	// �ٴ���Ʈ ������
	TArray<class ACharacter*> HittedCharacters;
	// ������
	TArray<class ACharacter*> StiffedCharacters;

	class ACGuardPoint* GuardPoint;

};

/*
	210828 ����

	�ؾ� �ϴ� ��
	Special ��� (��Ŭ��) �����

	Melee �� ����
	-> Capsule Trace�� ����
	�ε��� ���� ���̰�,
	���� �����̸�,
	������ ���� �� ���� ����

	(���� �� ������ �ƴ� �ٸ� ������ ���, �Ϲ� Hitted�� �� - ���ʿ� ���ǿ� ������ ������)
	(���忡 ����������, �������� �ſ� ���� ������ ��� (ex : player�� ���� �̻��� ü��),
	player�� ���� ���¿� ���� -> �ٸ� ����Ʈ ����� ���� (�ð��� �������� ���� �ɷ����� ��))

	����Ʈ ���� (���带 ������ ������ �ð��� ���)
	(���忡 �������� ��, �ð��� Ȯ���Ͽ� 0.3�� ���ܶ��,
	�и� ��ǰ� �Բ�, �ε��� ���� ���� ���·� ����)
	( + ��� �÷��̾�� ���� ���� �ο�???)

	���带 �Ѽհ˿��� �־�� �ϳ�??
	-> â�� ��˿� �ٸ� �� �ֱ⿡�� �ʹ� ���������,
	�ڵ带 �� ���� ����� ��

	����� ��ü�� ������ �ε�,
	bool ���� tick �̿�

	Montage�� ������, Up �������� �װ��� �ʿ�
	(��Ŭ���� ������, bool ���� false�� �ؾ� ��)

	UDamageType�� �߰��Ͽ�
	���� �ݰ��� ��, �ҷ��� Damage��
	���� ���·� ����

	������ Guard ���¸� ����� ��
	-> ��Ȯ���� Special��

	���� Special ���¿���
	���⿡ ���� Guard ���·� ��ȯ
	(-> ���ε��� ���� ���ֱ� ���ٴ� ���� ���¿� ���� blend�� �ϴ� ����
	bone ������ ���� ���� ����)
	(���� blend ���� ���� -> guard ���¿� ���� blend + ���� �ٸ� �ִϸ��̼�������,
	���� ���¿� ���� �з��� ������)

	���� ���¿�����
	Tick���� ���� ������ �Ǻ���

	+
	DoSpecial�� ���带 ������
	-> ���⼭�� ��Ÿ�ְ� �ʿ� ����

	begin_Special�� ����..?
	(�������� ����)

	������ Parrying��
	Guard �Լ��� ������ ��
	+ End_Special���� ���� ���·� �ǵ���

	��Ŭ���� �� ����
	������ �ʿ���
	(���� FlipFlop���� ������ٰ�, ���߿� ���� ������ �����ϱ⿡)

*/
