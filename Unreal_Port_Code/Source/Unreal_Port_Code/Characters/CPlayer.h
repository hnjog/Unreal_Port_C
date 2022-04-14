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
class UNREAL_PORT_CODE_API ACPlayer : public ACharacter, public IIAbleInventory, public IICharacter, public IGenericTeamAgentInterface
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

	UPROPERTY(EditDefaultsOnly)
		float FocusRecoveryRate = 0.1f;
private:
	//Scene Components
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UPostProcessComponent* PostProcess;

	//Actor Components
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCAblityComponent* Ability;

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

	FORCEINLINE class UCEquip_Arrow* GetEquipArrow() { return SelectedArrow; }

public:
	UFUNCTION(BlueprintImplementableEvent)
		void InInventroy();

	UFUNCTION(BlueprintImplementableEvent)
		void DoRush();

	UFUNCTION(BlueprintCallable)
		void End_Rush();

	UFUNCTION()
		void End_AirCombo();
private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnCharge(float InAxis);

private:
	void OnSprint();
	void OffSprint();

	void OnMelee();	// 차후 장비한 아이템에 따라서 Mode를 바꾸는 설정이 필요함 (Type 등의 요소를 통하여?) -> 혹은 액션 컴포넌트 내부에서 조정
	void OnBow();

	void OnAction();
	void OnSpecial();
	void OffSpecial();

	void OnDodge();
	void End_Dodge() override;

	void OnPickUp();
	void OnInventory();

	void OnFocus();

	void OnTarget();

	void OnRush();

	void CreateGhostTrail() override;

	UFUNCTION(BlueprintCallable, Category = "Items")
		void OnUseItem(class UCItem* item) override;
	// 인벤토리와 연관됨
	// 장착형 아이템이라면 장착을
	// 사용형 아이템이라면 사용

	void Hitted();
	void End_Hit() override;

	void Dead();
	void End_Dead() override;

	// if bDamage false - Healing
	void DamageUpdate(float amount, bool bDamage = true);
	void FocusUpdate(float amount, bool bSub = true);

	virtual void Stiff();
	virtual void End_Stiff() override;

	virtual void WakeUp();
	virtual void End_WakeUp() override;

	virtual void Parrying(bool result) override;
	virtual void Guard(bool result) override;

	void TimeFreeze();
	UFUNCTION()
		void TimeFreezeEnd();

	UFUNCTION()
		void DodgeSuccess();

	void AirComboState(bool val);


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

	UPROPERTY(VisibleAnywhere)
		class UCEquip_Arrow* SelectedArrow;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "widgets")
		TSubclassOf<class UCUserWidget_HealthBar> HealthWidgetClass;

	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_HealthBar* HealthWidget;

	UPROPERTY(EditDefaultsOnly, Category = "widgets")
		TSubclassOf<class UCUserWidget_FocusUI> FocusWidgetClass;

	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_FocusUI* FocusWidget;

	UPROPERTY(EditDefaultsOnly, Category = "widgets")
		TSubclassOf<class UCUserWidget_Aim> AimWidgetClass;

	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_Aim* AimWidget;

protected:
	bool bParrying;
	bool bGuard;

	bool bDamagedLastAttack;
	FHitResult HittedResult;

	float WakeUpTimer;

	class UMaterialInstanceDynamic* PostMat;

	float deltatime;

	float airTimer = 0.0f;

	bool waitAir = false;
};
