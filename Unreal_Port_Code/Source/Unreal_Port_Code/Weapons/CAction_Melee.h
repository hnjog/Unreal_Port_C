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
	// 콤보 구간 켯다 끄는 용
	FORCEINLINE void EnableCombo() { bEnableAttack = true; }
	FORCEINLINE void DisableCombo() { bEnableAttack = false; }

	// Player에서 OnSpecial 상태이며, SuccessGuard가 True인 상태인지를 판단
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
	//부모가 UFUCION 이면 자식도 해당 옵션을 지니고 상속이 됨
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

	//For Guard (어차피 Melee는 이걸로 통일할 생각이므로)
	bool bOnGuard = false;
	bool bParrying = false;
	bool bSuccessGuard = false;
	float GuardTimer = 0.0f;

	float HitStopRate = 0.2f;

	// 다단히트 방지용
	TArray<class ACharacter*> HittedCharacters;
	// 경직용
	TArray<class ACharacter*> StiffedCharacters;

	class ACSpecialPoint* GuardPoint;

};

/*
	궁중 공격의 재입력 대기 시간이 지난 경우, (combo)
	End_DoAction으로 들어오기에
	
	Idle State로 변환됨

	Idle 상태가 되는 경우,

	1. AirCombo 상태에 진입하였으나,
	1초 내에 재 공격이 이루어 지지 않음

	2. AirCombo의 정해진 횟수를 모두 공격함

	AirCombo End 등의 함수를 만들어 호출해야 함
	

*/
