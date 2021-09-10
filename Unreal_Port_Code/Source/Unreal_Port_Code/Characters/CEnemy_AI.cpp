#include "CEnemy_AI.h"
#include "Global.h"

ACEnemy_AI::ACEnemy_AI()
{
	Stance = EEnemyStance::Idle;

}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

}

void ACEnemy_AI::End_Dead()
{
	if(OnStanceTypeChanged.IsBound() == true)
		OnStanceTypeChanged.Clear();

	Super::End_Dead();
}

// 어쩌면 이거 자체를 하나의 컴포넌트로 만드는 것이 좋았을지도 모르곘지만
void ACEnemy_AI::SetIdleStance()
{
	ChangeStanceType(EEnemyStance::Idle);
}

void ACEnemy_AI::SetBattleStance()
{
	ChangeStanceType(EEnemyStance::Battle);
}

void ACEnemy_AI::SetDoubtStance()
{
	ChangeStanceType(EEnemyStance::Doubt);
}

void ACEnemy_AI::SetHittedStance()
{
	ChangeStanceType(EEnemyStance::Hitted);
}

void ACEnemy_AI::ChangeStanceType(EEnemyStance InNewType)
{
	EEnemyStance prev = Stance;
	Stance = InNewType;

	if (OnStanceTypeChanged.IsBound())
		OnStanceTypeChanged.Broadcast(prev, InNewType);

}

/*
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));


	OnStanceTypeChanged 이걸 어느 타이밍에 BroadCast 해줄까?
	-> ChangeType 같은 녀석을 하나 만들고,
	거기서 broad cast 해주는 것이 좋아보이긴 함
	(그리고 type에 맞는 함수들을 만들어서
	다른 클래스에서 호출하게 하는것)

*/