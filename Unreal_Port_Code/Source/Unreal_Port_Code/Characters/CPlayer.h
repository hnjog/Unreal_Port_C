#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Items/CEquipItem.h"
#include "Items/CEquipItem_Weapon.h"			// Melee�� ����� Ÿ���� ���ϱ� ���Ͽ�
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

	// �̰� �ʿ� �����ʳ�? -> ȭ�� ���̰� �ϴ� ��Ƽ���̿��� ȣ���ϴ� �뵵�� ���Ѽ� ����
	// ��� ��Ƽ���� Ŭ���� �����, ���� �ʿ䰡 �ֱ� �� (IIAbleInventory �� �̰� ������ ������)
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

	void OnMelee();	// ���� ����� �����ۿ� ���� Mode�� �ٲٴ� ������ �ʿ��� (Type ���� ��Ҹ� ���Ͽ�?) -> Ȥ�� �׼� ������Ʈ ���ο��� ����
	void OnBow();

	void OnAction();
	void OnSpecial();
	void OffSpecial();

	void OnPickUp();
	void OnInventory();

	UFUNCTION(BlueprintCallable, Category = "Items")
		void OnUseItem(class UCItem* item) override;
	// �κ��丮�� ������
	// ������ �������̶�� ������
	// ����� �������̶�� ���

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

	// ��� ���⼭ �������ִ°� ���� ���ϱ�?
	// Ȱ ���⸦ ���� ���� ����? -> ���� �׳� ȭ�� �����ؼ� ��� ���� �ʳ�
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
	�÷��̾� ���忡 ���Ͽ�
	��Ȯ���� Action ���� Special

	���¿� stiff �� �ϴ� 
	���� Ȥ�� ���� ���¸� �߰��� ��,
	�и��� �����ϸ�,
	�ش� ���·� ������,
	(attachment �� off collision �ؾ� �ϰ�,
	��Ÿ�� ��� ���� �ϱ�,
	bst ������ �켱 ������ dead -> Hitted -> stiff ������)


	GetOwner�� ���� Capsule�� ũ�� ���� ���� ��,

	1. ����� state�� action ��������
	2. attachment�� ã�Ҵ��� ���� �����

	����Ʈ ����??

	��Ŭ���� ���� ������ �ð��� ������ ���ϰ�,
	�� 0.3~0.5 �� ���� ����� ������ ������ �̶� �����ϴ� ����?




*/