#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include"Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CAblityComponent.h"
#include"Components/CActionComponent.h"
#include"Components/PostProcessComponent.h"
#include"Items/CInventoryComponent.h"
#include"Items/CItem.h"
#include"Items/CConsumeItem.h"
#include"Items/CItemFiled.h"
#include "Items/CEquip_Arrow.h"
#include "Items/CItem_Arrow.h"
#include<Weapons/CDamageType_SpecialAttack.h>
#include<Widgets/CUserWidget_HealthBar.h>
#include<Widgets/CUserWidget_FocusUI.h>
#include<Widgets/CUserWidget_Aim.h>
#include<Ablities/CGhostTrail.h>
#include<Ablities/CDodgePoint.h>
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include"Interfaces/ITargetAble.h"
//#include "CableComponent.h"
//#include "Components/SplineComponent.h"

ACPlayer::ACPlayer()
	:bDamagedLastAttack(false), WakeUpTimer(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	//CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetRootComponent());
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateComponent(this, &PostProcess, "PostProcess", GetRootComponent());
	//CHelpers::CreateComponent(this, &Grapple, "Grapple", GetMesh());
	//CHelpers::CreateComponent(this, &EndPoint, "EndPoint", GetMesh());
	//CHelpers::CreateComponent(this, &Spline, "Spline", GetRootComponent());

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Inventory, "Inventory");
	CHelpers::CreateActorComponent(this, &Ability, "Ability");

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

	CHelpers::GetClass<UCUserWidget_HealthBar>(&HealthWidgetClass, "WidgetBlueprint'/Game/Widzets/WBP_PlayerHealth.WBP_PlayerHealth_C'");
	CHelpers::GetClass<UCUserWidget_FocusUI>(&FocusWidgetClass, "WidgetBlueprint'/Game/Widzets/WBP_PlayerFocus.WBP_PlayerFocus_C'");
	CHelpers::GetClass<UCUserWidget_Aim>(&AimWidgetClass, "WidgetBlueprint'/Game/Widzets/WB_BowAim.WB_BowAim_C'");

	Ability->OnTimeFreezeEnd.AddDynamic(this, &ACPlayer::TimeFreezeEnd);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACPlayer::OnComponentHit);

	if (!!GetController())
	{
		HealthWidget = CreateWidget<UCUserWidget_HealthBar, APlayerController>(GetController<APlayerController>(), HealthWidgetClass);
		HealthWidget->AddToViewport();

		HealthWidget->SetVisible(true);
		HealthWidget->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());

		FocusWidget = CreateWidget<UCUserWidget_FocusUI, APlayerController>(GetController<APlayerController>(), FocusWidgetClass);
		FocusWidget->AddToViewport();

		FocusWidget->SetVisible(true);
		FocusWidget->UpdateFocus(Status->GetFocusAmount(), Status->GetMaxFocusAmount());

		AimWidget = CreateWidget<UCUserWidget_Aim, APlayerController>(GetController<APlayerController>(), AimWidgetClass);
		AimWidget->AddToViewport();

		AimWidget->OffAim();
	}

	UMaterialInstanceConstant* postmat;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&postmat, "MaterialInstanceConstant'/Game/Material/Effect/M_Shadow_Test_Inst.M_Shadow_Test_Inst'");

	PostMat = UMaterialInstanceDynamic::Create(postmat, this);
	PostMat->SetScalarParameterValue("Overlay", 1.0f);

	PostProcess->Settings.AddBlendable(PostMat, 0.0f);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	deltatime = DeltaTime;

	if (Status->NotFullFocus() == true && Ability->IsTimeFreeze() == false)
	{
		FocusUpdate(FocusRecoveryRate * DeltaTime, false);
	}

	ACItemFiled* filedItem = nullptr;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * 30.0f;
	TArray<AActor*> actors;
	TArray<FHitResult> hitResults;

	bool result = UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), start, end, 60.0f, 96.0f, ETraceTypeQuery::TraceTypeQuery1, false, actors, EDrawDebugTrace::None, hitResults, true);
	if (result == true)
	{
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

	if (State->IsKnockOutMode() == true)
	{
		Popcorn();

		CheckTrue(GetCharacterMovement()->IsFalling());

		WakeUpTimer += DeltaTime;

		if (WakeUpTimer >= WakeUpTime && FMath::IsNearlyZero(GetVelocity().Size()) == true) //
		{
			State->SetWakeUpMode();
			WakeUpTimer = 0.0f;
			return;
		}
	}

	if (Ability->IsTargeting() == true && !!Ability->GetTarget())
	{
		AActor* target = Ability->GetTargetActor();

		CheckNull(target);
		float distance = GetDistanceTo(target);

		if (distance <= Ability->GetDistance())
		{
			FRotator rotator;
			rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
			rotator.Roll = Camera->GetComponentRotation().Roll;

			GetController()->SetControlRotation(rotator);
		}
		else
		{
			Ability->ClearTargets();
		}
	}

	if (waitAir == true)
	{
		airTimer += DeltaTime;

		if (airTimer >= 2.0f)
		{
			End_AirCombo();
			airTimer = 0.0f;
		}
	}

	if (State->IsAirComboWaitMode() == true && GetCharacterMovement()->IsFalling() == false)
	{
		State->SetIdleMode();
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
	PlayerInputComponent->BindAxis("Charge", this, &ACPlayer::OnCharge);

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

	PlayerInputComponent->BindAction("FocusMode", EInputEvent::IE_Pressed, this, &ACPlayer::OnFocus);

	PlayerInputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, &ACPlayer::OnDodge);

	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, this, &ACPlayer::OnTarget);

	PlayerInputComponent->BindAction("Rush", EInputEvent::IE_Pressed, this, &ACPlayer::OnRush);

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
	CheckTrue(State->IsRushMode());
	CheckTrue(State->IsAirComboMode());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());
	CheckTrue(State->IsRushMode());
	CheckTrue(State->IsAirComboMode());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	CheckTrue(State->IsRushMode());
	CheckTrue(State->IsAirComboMode());

	AddControllerYawInput(InAxis * HorizontalRate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	CheckTrue(State->IsRushMode());
	CheckTrue(State->IsAirComboMode());

	AddControllerPitchInput(InAxis * VerticalRate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnCharge(float InAxis)
{
	CheckFalse(Action->IsBowMode());
	CheckFalse(State->IsSpecialMode());

	float value = InAxis * deltatime;

	Action->AddCharge(value);

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
	if (State->IsRushMode() == true)
	{
		waitAir = false;
		State->SetAirComboWaitMode();
	}

	Action->DoAction();
}

void ACPlayer::OnSpecial()
{
	if (Action->IsBowMode())
	{
		AimWidget->OnAim();
	}
	Action->DoSpecial();
}

void ACPlayer::OffSpecial()
{
	if (Action->IsBowMode())
	{
		AimWidget->OffAim();
		Action->ReleaseSpecial();
	}
	else
	{
		Action->CancelSpecial();
	}

}

void ACPlayer::OnDodge()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->CanFocus());
	FocusUpdate(1.0f);

	ACDodgePoint* dodgePoint = GetWorld()->SpawnActorDeferred<ACDodgePoint>(ACDodgePoint::StaticClass(), GetMesh()->GetComponentTransform(), this);
	dodgePoint->OnSuccessDodge.BindUFunction(this, "DodgeSuccess");
	UGameplayStatics::FinishSpawningActor(dodgePoint, GetMesh()->GetComponentTransform());

	State->SetDodgeMode();
	Status->SetStop();
	Montages->PlayDodge();
}

void ACPlayer::End_Dodge()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACPlayer::OnPickUp()
{
	CheckFalse(State->IsIdleMode());	// 때리거나 맞으면서는 아이템 줍지 못하도록
	ACItemFiled* filedItem = nullptr;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + GetActorForwardVector() * 30.0f;
	TArray<AActor*> actors;
	TArray<FHitResult> hitResults;

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

void ACPlayer::OnFocus()
{
	CheckFalse(Status->CanFocus());
	CheckTrue(Ability->IsTimeFreeze());

	TimeFreeze();
	FocusUpdate(1.0f);
}

void ACPlayer::OnTarget()
{
	if (Ability->IsTargeting() == false)
	{
		if (Action->IsBowMode())
			AimWidget->OnTarget();

		Ability->SetTarget();
	}
	else
	{
		if (Action->IsBowMode())
			AimWidget->OffTarget();

		Ability->ClearTargets();
	}
}

void ACPlayer::OnRush()
{
	CheckNull(Ability->GetTargetActor());

	State->SetRushMode();

	if (Ability->IsTargetKnoukout() == true)
	{
		TimeFreeze();
	}

	Ability->TargetBeaten();

	DoRush();
}

void ACPlayer::End_Rush()
{
	if (Ability->IsTimeFreeze() == true)
	{
		waitAir = true;
		AirComboState(true);

		FVector loc = GetActorLocation() + FVector(0, 0, -20);
		SetActorLocation(loc);
	}
	else
	{
		waitAir = false;
		AirComboState(false);

		State->SetIdleMode();
	}

}

void ACPlayer::End_AirCombo()
{
	State->SetAirComboWaitMode();

	Ability->EndTargetBeaten();
	Ability->ClearTargets();

	Status->SetMove();
	airTimer = 0.0f;
	waitAir = false;
	AirComboState(false);
}

void ACPlayer::CreateGhostTrail()
{
	ACGhostTrail* ghostTrail = GetWorld()->SpawnActorDeferred<ACGhostTrail>(ACGhostTrail::StaticClass(), GetMesh()->GetComponentTransform());
	ghostTrail->SetSkeletal(GetMesh()->SkeletalMesh);
	ghostTrail->Init(GetMesh());
	UGameplayStatics::FinishSpawningActor(ghostTrail, GetMesh()->GetComponentTransform());
}

void ACPlayer::OnUseItem(class UCItem* item)
{
	if (item)
	{
		item->Use(this);
		item->OnUse(this);	// bi
	}

}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CheckTrueResult(State->IsDodgeMode(), Status->GetHealth());
	CheckTrueResult(State->IsWakeUpMode(), Status->GetHealth());
	CheckTrueResult(State->IsKnockOutMode(), Status->GetHealth());
	CheckTrueResult(Ability->IsTimeFreeze(), Status->GetHealth());

	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	if (bParrying == true)
	{
		bParrying = false;
		return Status->GetHealth();
	}

	Status->SetStop();
	if (Action->IsBowMode())
	{
		AimWidget->OffAim();
		AimWidget->OffTarget();
	}

	if (bGuard == true)
	{
		damage *= 0.25f;
		DamageUpdate(damage);

		if (DamageEvent.DamageTypeClass == UCDamageType_SpecialAttack::StaticClass())
		{
			Action->CancelSpecial();
			State->SetStiffMode();
		}

		bGuard = false;
		return Status->GetHealth();
	}

	if (DamageEvent.DamageTypeClass == UCDamageType_SpecialAttack::StaticClass())
	{
		Action->CancelSpecial();
		State->SetKnockOutMode();
		bDamagedLastAttack = true;
		DamageUpdate(damage);
		return Status->GetHealth();
	}

	Action->AbortByDamage();

	DamageUpdate(damage);

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
	Status->SetMove();
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

void ACPlayer::DamageUpdate(float amount, bool bDamage)
{
	if (bDamage)
	{
		Status->SubHealth(amount);
	}
	else
	{
		Status->AddHealth(amount);
	}
	HealthWidget->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
}

void ACPlayer::FocusUpdate(float amount, bool bSub)
{
	if (bSub)
	{
		Status->SubFocus(amount);
	}
	else
	{
		Status->AddFocus(amount);
	}
	FocusWidget->UpdateFocus(Status->GetFocusAmount(), Status->GetMaxFocusAmount());
}

void ACPlayer::Stiff()
{
	Montages->PlayStiff();
}

void ACPlayer::End_Stiff()
{
	Status->SetMove();
	State->SetIdleMode();
}

void ACPlayer::WakeUp()
{
	End_Popcorn();
	Montages->PlayWakeUp();
}

void ACPlayer::End_WakeUp()
{
	Status->SetMove();
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

void ACPlayer::TimeFreeze()
{
	PostProcess->Settings.AddBlendable(PostMat, 1.0f);

	Ability->TimeFreeze();
}

void ACPlayer::TimeFreezeEnd()
{
	PostProcess->Settings.AddBlendable(PostMat, 0.0f);
}

void ACPlayer::DodgeSuccess()
{
	CheckTrue(Ability->IsTimeFreeze());

	FocusUpdate(1.0f, false);
	TimeFreeze();
}

void ACPlayer::AirComboState(bool val)
{
	if (val == true)
	{
		GetMesh()->SetEnableGravity(false);
		GetCharacterMovement()->GravityScale = 0.0f;
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		return;
	}

	GetMesh()->SetEnableGravity(true);
	GetCharacterMovement()->GravityScale = 1.0f;

}

void ACPlayer::LaunchByHitted()
{
	CheckFalse(bDamagedLastAttack);
	bDamagedLastAttack = false;

	FVector up = FVector::UpVector;

	FVector Direction = HittedResult.ImpactNormal * LauchValue + up * LauchUpValue;

	LaunchCharacter(Direction, true, false);
}

void ACPlayer::Popcorn()
{
	StopAnimMontage();

	GetMesh()->SetWorldLocation(GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	GetMesh()->AddWorldRotation(FRotator(0, 0, -2.0f), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetSimulatePhysics(true);
}

void ACPlayer::End_Popcorn()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetWorldLocation(GetActorLocation());

	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
	GetMesh()->SetWorldTransform(GetActorTransform());

	GetMesh()->AddRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
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

void ACPlayer::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (State->IsKnockOutMode() == true)
	{
		HittedResult = Hit;
		LaunchByHitted();
	}
}
