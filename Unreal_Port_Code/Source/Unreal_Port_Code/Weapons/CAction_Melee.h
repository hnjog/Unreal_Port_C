#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAction.h"
#include "CAction_Melee.generated.h"

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

	//For Guard (어차피 Melee는 이걸로 통일할 생각이므로)
	bool bOnGuard = false;
	bool bParrying = false;
	bool bSuccessGuard = false;
	float GuardTimer = 0.0f;

	// 다단히트 방지용
	TArray<class ACharacter*> HittedCharacters;
	// 경직용
	TArray<class ACharacter*> StiffedCharacters;

	class ACGuardPoint* GuardPoint;

};

/*
	210828 정리

	해야 하는 것
	Special 기능 (우클릭) 만들기

	Melee 는 가드
	-> Capsule Trace를 통해
	부딪힌 것이 적이고,
	공격 상태이면,
	데미지 감소 및 경직 상태

	(가드 중 전방이 아닌 다른 방향인 경우, 일반 Hitted가 됨 - 애초에 조건에 들어오지 않을듯)
	(가드에 성공했으나, 데미지가 매우 강한 공격인 경우 (ex : player의 절반 이상의 체력),
	player는 경직 상태에 빠짐 -> 다만 퍼펙트 가드는 예외 (시간을 기준으로 먼저 걸러내야 함))

	퍼펙트 가드 (가드를 시작한 순간의 시간을 잰다)
	(가드에 성공했을 때, 시간을 확인하여 0.3초 내외라면,
	패링 모션과 함께, 부딪힌 적을 경직 상태로 만듦)
	( + 잠시 플레이어에게 무적 상태 부여???)

	가드를 한손검에만 넣어야 하나??
	-> 창과 대검에 다른 걸 넣기에는 너무 장대해지고,
	코드를 또 갈아 엎어야 함

	스페셜 자체는 내버려 두되,
	bool 값과 tick 이용

	Montage를 누르며, Up 했을때의 그것이 필요
	(우클릭을 땟을때, bool 값을 false로 해야 함)

	UDamageType을 추가하여
	으로 반격할 때, 소량의 Damage와
	경직 상태로 만듦

	요점은 Guard 상태를 만드는 것
	-> 정확히는 Special임

	따라서 Special 상태에는
	무기에 따라 Guard 상태로 전환
	(-> 따로따로 블렌드 해주기 보다는 무기 상태에 따라서 blend를 하는 것이
	bone 개수를 적게 먹을 것임)
	(무기 blend 끝난 이후 -> guard 상태에 따른 blend + 각기 다른 애니메이션이지만,
	무기 상태에 따라서 분류가 가능함)

	가드 상태에서는
	Tick으로 적의 공격을 판별함

	+
	DoSpecial은 가드를 시작함
	-> 여기서는 몽타주가 필요 없음

	begin_Special은 딱히..?
	(내버려는 두자)

	오히려 Parrying과
	Guard 함수를 만들어야 함
	+ End_Special에서 원래 상태로 되돌림

	우클릭을 땔 때의
	반응이 필요함
	(괜히 FlipFlop으로 만들었다가, 나중에 꼬일 염려가 존재하기에)

*/
