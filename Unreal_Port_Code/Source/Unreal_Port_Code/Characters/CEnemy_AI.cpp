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

// ��¼�� �̰� ��ü�� �ϳ��� ������Ʈ�� ����� ���� ���������� �𸣁�����
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


	OnStanceTypeChanged �̰� ��� Ÿ�ֿ̹� BroadCast ���ٱ�?
	-> ChangeType ���� �༮�� �ϳ� �����,
	�ű⼭ broad cast ���ִ� ���� ���ƺ��̱� ��
	(�׸��� type�� �´� �Լ����� ����
	�ٸ� Ŭ�������� ȣ���ϰ� �ϴ°�)

*/