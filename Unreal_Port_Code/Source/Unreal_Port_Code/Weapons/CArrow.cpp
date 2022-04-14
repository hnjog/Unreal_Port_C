#include "CArrow.h"
#include"Global.h"
#include<Components/StaticMeshComponent.h>
#include<Components/SphereComponent.h>
#include<GameFramework/ProjectileMovementComponent.h>
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include<Particles/ParticleSystemComponent.h>

ACArrow::ACArrow()
{
	CHelpers::CreateComponent(this, &StaticMesh, "StaticMesh");
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", StaticMesh);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Trail, "Trail", StaticMesh);

	//Actor
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Weapons/_Bow/Meshes/SM_ElvenArrow.SM_ElvenArrow'");
	StaticMesh->SetStaticMesh(mesh);
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetEnableGravity(false);
	StaticMesh->SetGenerateOverlapEvents(false);

	Sphere->SetRelativeLocation(FVector(80.0f, 0.0f, 0.0f));
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetSphereRadius(20.0f);
	Sphere->bHiddenInGame = false;

	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/Particles/P_ky_trail_shine.P_ky_trail_shine'");
	Trail->SetTemplate(particle);

	Projectile->SetUpdatedComponent(StaticMesh);
	Projectile->InitialSpeed = 4000.0f;
	Projectile->MaxSpeed = 8000.0f;
	Projectile->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 10.0f;
}


void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACArrow::OnComponentBeginOverlap);
	OnDestroyed.AddDynamic(this, &ACArrow::OnDestoryEvent);

	UMaterialInstanceConstant* effectMat;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&effectMat, "MaterialInstanceConstant'/Game/Material/MI_Arrow.MI_Arrow'");

	EffectMat = UMaterialInstanceDynamic::Create(effectMat, this);

	if (bCharged == true)
	{
		EffectMat->SetScalarParameterValue("Intensity", 10.0f);
		EffectMat->SetScalarParameterValue("BeatSpeed", 100.0f);
		EffectMat->SetScalarParameterValue("BeatScale", 5.0f);
	}
	else
	{
		EffectMat->SetScalarParameterValue("Intensity", 0.0f);
		EffectMat->SetScalarParameterValue("BeatSpeed", 0.0f);
		EffectMat->SetScalarParameterValue("BeatScale", 0.0f);
	}

	//BeatSpeed
	StaticMesh->SetMaterial(0, EffectMat);

	Trail->BeginTrails("TrailStart", "TrailEnd", ETrailWidthMode::ETrailWidthMode_FromFirst, 5.0f);
}

void ACArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());

	if (OnThrowOverlap.IsBound())
		OnThrowOverlap.Broadcast(SweepResult, bCharged);

	if (bCharged == false)
	{
		OnThrowOverlap.Clear();
	}
	Destroy();
}

void ACArrow::OnDestoryEvent(AActor* DestroyedActor)
{
	Trail->EndTrails();
	OnThrowOverlap.Clear();
}
