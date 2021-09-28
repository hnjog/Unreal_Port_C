#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Items/CEquipItem.h"
#include "Items/CEquipItem_Weapon.h"			// Melee에 사용할 타입을 정하기 위하여
#include "Interfaces/IAbleInventory.h"
#include "Interfaces/ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACPlayer : public ACharacter , public IIAbleInventory, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 0;

	UPROPERTY(EditDefaultsOnly)
		float LauchValue = 100.0f;

	UPROPERTY(EditDefaultsOnly)
		float LauchUpValue = 30.0f;

	UPROPERTY(EditDefaultsOnly)
		float WakeUpTime = 1.0f;
private:
	//Scene Components
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//Actor Components
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;

public:
	ACPlayer();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const;

	UFUNCTION()
		void SetEquipItem(class UCEquipItem* EquipItem, bool result) override;

	// 이거 필요 없지않나? -> 화살 보이게 하는 노티파이에서 호출하는 용도로 냅둘수 있음
	// 요거 노티파이 클래스 만든뒤, 얻어올 필요가 있긴 함 (IIAbleInventory 에 이걸 넣을까 생각중)
	FORCEINLINE class UCEquip_Arrow* GetEquipArrow() { return SelectedArrow; }

public:
	UFUNCTION(BlueprintImplementableEvent)
		void InInventroy();

private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

private:
	void OnSprint();
	void OffSprint();

	void OnMelee();	// 차후 장비한 아이템에 따라서 Mode를 바꾸는 설정이 필요함 (Type 등의 요소를 통하여?) -> 혹은 액션 컴포넌트 내부에서 조정
	void OnBow();

	void OnAction();
	void OnSpecial();
	void OffSpecial();

	void OnPickUp();
	void OnInventory();

	UFUNCTION(BlueprintCallable, Category = "Items")
		void OnUseItem(class UCItem* item) override;
	// 인벤토리와 연관됨
	// 장착형 아이템이라면 장착을
	// 사용형 아이템이라면 사용

	void Hitted();
	void End_Hit() override;

	void Dead();
	void End_Dead() override;

	virtual void Stiff();
	virtual void End_Stiff() override;

	virtual void WakeUp();
	virtual void End_WakeUp() override;

	virtual void Parrying(bool result) override;
	virtual void Guard(bool result) override;

protected:
	virtual void LaunchByHitted() override;
	virtual void Popcorn() override;
	virtual void End_Popcorn() override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	float HorizontalRate = 45.0f;
	float VerticalRate = 45.0f;

private:
	class AController* DamageInstigator;

	// player Item
	UPROPERTY(VisibleAnywhere)
		UCEquipItem_Weapon* MainWeapon;

	UPROPERTY(VisibleAnywhere)
		UCEquipItem* BowWeapon;

	// 얘는 여기서 설정해주는게 나은 편일까?
	// 활 무기를 적이 쓸걸 생각? -> 적은 그냥 화살 생성해서 쏘면 되지 않나
	UPROPERTY(VisibleAnywhere)
		class UCEquip_Arrow* SelectedArrow;

protected:
	bool bParrying;
	bool bGuard;

	bool bDamagedLastAttack;
	FHitResult HittedResult;

	float WakeUpTimer;
};

/*
	플레이어 가드에 대하여
	정확히는 Action 쪽의 Special

	상태에 stiff 라 하는 
	경직 혹은 기절 상태를 추가한 뒤,
	패링에 성공하면,
	해당 상태로 가도록,
	(attachment 를 off collision 해야 하고,
	몽타주 재생 정도 일까,
	bst 에서의 우선 순위는 dead -> Hitted -> stiff 정도로)


	GetOwner를 통해 Capsule의 크기 등을 얻어온 뒤,

	1. 대상의 state가 action 상태인지
	2. attachment를 찾았는지 등을 고려함

	퍼펙트 가드??

	우클릭을 누른 순간의 시간의 정도를 구하고,
	약 0.3~0.5 초 내에 대상의 공격이 닿으면 이라 판정하는 것은?




*/