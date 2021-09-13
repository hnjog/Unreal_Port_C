#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include"Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include"Components/CActionComponent.h"
#include"Items/CInventoryComponent.h"
#include"Items/CItem.h"
#include"Items/CConsumeItem.h"
#include"Items/CItemFiled.h"
#include "Items/CEquip_Arrow.h"
#include "Items/CItem_Arrow.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Inventory, "Inventory");

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/Archer/erika_archer.erika_archer'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> animClass;
	CHelpers::GetClass<UAnimInstance>(&animClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animClass);

	bUseControllerRotationYaw = false;
	SpringArm->SetRelativeLocation(FVector(0, 0, 50));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	Inventory->Capacity = 20;
	Inventory->OnEquipChanged.AddDynamic(Action, &UCActionComponent::OnChangeItem);
	Inventory->OnEquipChanged.AddDynamic(this, &ACPlayer::SetEquipItem);

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACItemFiled* filedItem = nullptr;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * 30.0f;
	TArray<AActor*> actors;
	TArray<FHitResult> hitResults;

	bool result = UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), start, end, 60.0f, 96.0f, ETraceTypeQuery::TraceTypeQuery1, false, actors, EDrawDebugTrace::None, hitResults, true);
	CheckFalse(result);

	for (auto hit : hitResults)
	{
		filedItem = Cast<ACItemFiled>(hit.GetActor());

		if (filedItem != nullptr)
		{
			ACItem_Arrow* arrow = Cast<ACItem_Arrow>(filedItem);
			if (!!arrow)
			{
				if (arrow->GetStucked() == false)
				{
					continue;
				}
			}
			filedItem->OnVisible();
		}
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	//Action
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACPlayer::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACPlayer::OffSprint);

	PlayerInputComponent->BindAction("Melee", EInputEvent::IE_Pressed, this, &ACPlayer::OnMelee);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, this, &ACPlayer::OnBow);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction);
	PlayerInputComponent->BindAction("Special", EInputEvent::IE_Pressed, this, &ACPlayer::OnSpecial);
	PlayerInputComponent->BindAction("Special", EInputEvent::IE_Released, this, &ACPlayer::OffSpecial);

	PlayerInputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &ACPlayer::OnPickUp);

	PlayerInputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ACPlayer::OnInventory);
}

void ACPlayer::SetEquipItem(UCEquipItem* EquipItem, bool result)
{
	if (result == true)
	{
		if (EquipItem->EquipType == EEquipType::MainWeapon)
		{
			if (!!MainWeapon) MainWeapon->SetEquip(false);

			MainWeapon = Cast<UCEquipItem_Weapon>(EquipItem);
			return;
		}

		if (EquipItem->EquipType == EEquipType::Bow)
		{
			if (!!BowWeapon) BowWeapon->SetEquip(false);

			BowWeapon = EquipItem;
			return;
		}

		if (EquipItem->EquipType == EEquipType::Arrow)
		{
			if (!!SelectedArrow) SelectedArrow->SetEquip(false);

			SelectedArrow = Cast<UCEquip_Arrow>(EquipItem);
			return;
		}

	}
	else
	{
		if (EquipItem->EquipType == EEquipType::MainWeapon)
		{
			MainWeapon = nullptr;
			return;
		}

		if (EquipItem->EquipType == EEquipType::Bow)
		{
			BowWeapon = nullptr;
			return;
		}

		if (EquipItem->EquipType == EEquipType::Arrow)
		{
			SelectedArrow = nullptr;
			return;
		}
	}

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	AddControllerYawInput(InAxis * HorizontalRate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	AddControllerPitchInput(InAxis * VerticalRate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
}

void ACPlayer::OffSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
}

void ACPlayer::OnMelee()
{
	// 무기를 꺼내게 하는 것은 1번을 눌렀을때 해야하는 행동은 맞음
	// 다만 main으로 아이템이 존재해야 하지 않을까?
	// 장비 계열 아이템이 필요하다 생각함
	CheckFalse(State->IsIdleMode());
	if (MainWeapon == nullptr)
	{
		Action->SetUnarmedMode();
		return;
	}

	switch (MainWeapon->WeaponType)
	{
	case EMainWeapon::Blade: Action->SetBladeMode();
		break;
	case EMainWeapon::GreatSword: Action->SetTwoHandMode();
		break;
	case EMainWeapon::Spear: Action->SetSpearMode();
		break;
	default:
		Action->SetUnarmedMode();
	}

}

void ACPlayer::OnBow()
{
	CheckFalse(State->IsIdleMode());
	CheckNull(BowWeapon);

	Action->SetBowMode();
}

void ACPlayer::OnAction()
{
	Action->DoAction();
}

void ACPlayer::OnSpecial()
{
	Action->DoSpecial();
}

void ACPlayer::OffSpecial()
{
	Action->CancelSpecial();
}

void ACPlayer::OnPickUp()
{
	CheckFalse(State->IsIdleMode());	// 때리거나 맞으면서는 아이템 줍지 못하도록
	ACItemFiled* filedItem = nullptr;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * 30.0f;
	TArray<AActor*> actors;
	TArray<FHitResult> hitResults;

	//bool result = UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), start, end, 40.0f, 88.0f, ETraceTypeQuery::TraceTypeQuery1, false, actors, EDrawDebugTrace::ForDuration, hitResults, true);
	bool result = UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), start, end, 60.0f, 96.0f, ETraceTypeQuery::TraceTypeQuery1, false, actors, EDrawDebugTrace::ForDuration, hitResults, true);
	CheckFalse(result);

	for (auto hit : hitResults)
	{
		filedItem = Cast<ACItemFiled>(hit.GetActor());

		if (filedItem != nullptr)
		{
			break;
		}
	}

	if (filedItem == nullptr) return;

	Inventory->AddItem(filedItem->GetItem());
	filedItem->UnActive();

}

void ACPlayer::OnInventory()
{
	CheckFalse(State->IsIdleMode());

	InInventroy();
}

void ACPlayer::OnUseItem(class UCItem* item)
{
	if (item)
	{
		item->Use(this);
		item->OnUse(this);	// bi
	}

	/*CheckNull(Cast<UCConsumeItem>(item));

	Inventory->RemoveItem(item);*/

}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	CLog::Print(bParrying ? "Parrying" : "Not Parrying");
	CLog::Print(bGuard ? "Guard" : "Not Guard");

	// 여기에 패링 상태일때 TakeDamage를 리턴시키는 부분 필요
	if (bParrying == true)
	{
		bParrying = false;
		return Status->GetHealth();
	}

	if (bGuard == true)
	{
		Status->SubHealth(damage * 0.25f);
		bGuard = false;
		return Status->GetHealth();
	}

	Action->AbortByDamage();

	// 여기는 다운 상태

	// 여기는 경직 상태

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACPlayer::Hitted()
{
	Status->SetMove();

	Montages->PlayHitted();
}

void ACPlayer::End_Hit()
{
	State->SetIdleMode();
}

void ACPlayer::Dead()
{
	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDead();
}

void ACPlayer::End_Dead()
{
	Action->End_Dead();

	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void ACPlayer::Stiff()
{
	Montages->PlayStiff();
}

void ACPlayer::End_Stiff()
{
	State->SetIdleMode();
}

void ACPlayer::WakeUp()
{

}

void ACPlayer::End_WakeUp()
{
	State->SetIdleMode();
}

void ACPlayer::Parrying(bool result)
{
	bParrying = result;
}

void ACPlayer::Guard(bool result)
{
	bGuard = result;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	case EStateType::Stiff: Stiff(); break;
	case EStateType::WakeUp: WakeUp(); break;
	}
}
