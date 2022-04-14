#include "CEnemy_AI.h"
#include "Global.h"
#include<particles/ParticleSystemComponent.h>

ACEnemy_AI::ACEnemy_AI()
{
	Stance = EEnemyStance::Idle;

	CHelpers::CreateComponent(this, &ParticleSystem, "ParticleSystem", GetMesh());

	ParticleSystem->SetWorldScale3D(ParticleSize);
	ParticleSystem->SetupAttachment(GetMesh());
	ParticleSystem->bAutoActivate = false;
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	if (!!SpecialParticle)
	{
		ParticleSystem->SetTemplate(SpecialParticle);
	}
}

void ACEnemy_AI::End_Dead()
{
	if(OnStanceTypeChanged.IsBound() == true)
		OnStanceTypeChanged.Clear();

	Super::End_Dead();
}

void ACEnemy_AI::Idle()
{
	Super::Idle();
}

void ACEnemy_AI::Special()
{
	Super::Special();
	ParticleSystem->ToggleActive();
}

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
