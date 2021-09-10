#include "CItem_Arrow.h"
#include"Global.h"
#include<Components/SphereComponent.h>
#include<GameFramework/ProjectileMovementComponent.h>

ACItem_Arrow::ACItem_Arrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", StaticMesh);

	//Actor
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Sphere->SetRelativeLocation(FVector(80.0f, 0.0f, 0.0f));
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetSphereRadius(20.0f);
	Sphere->bHiddenInGame = false;

	Projectile->SetUpdatedComponent(StaticMesh);
	Projectile->InitialSpeed = 4000.0f;
	Projectile->MaxSpeed = 8000.0f;
	Projectile->ProjectileGravityScale = 0.5f;

	// Initial speed �� velocity
	// 0�̻��̸� �ʱ� velocity���� �����ϰ� velocity�� �������� ó����

	InitialLifeSpan = 10.0f;

}

void ACItem_Arrow::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACItem_Arrow::OnComponentBeginOverlap);

	// nocollision���� �ؾ� �� ����
	// ���� ��򰡿� �ε����� ������Ű�� ������ �ʿ���
	// + �� ��� StaticMesh�� Items���� �ٲ��, Simulate �� false�� �ٲ�� �����Ǿ�� �ϰ���
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetSimulatePhysics(false);

	//Item->Count = 1;
}

void ACItem_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �ᱹ tick���� sphere trace�� �ؾ� �ҵ�?

	if (bStucked == false)
	{
		FVector start = StaticMesh->GetSocketLocation("ArrowStart");
		FVector end = StaticMesh->GetSocketLocation("ArrowEnd");

		bool result = SphereTrace(start, end);
		if (result == true)
		{
			Stucked();
		}
	}

}

void ACItem_Arrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(bStucked);				// bStucked �� true�̸� �̹� ��򰡿� �����ִٴ� ��

	CLog::Print("Hit",1);
	CLog::Log(OtherActor);

	if (!!HitEffect)
	{
		FTransform transform = HitEffectTransform;
		transform.AddToTranslation(GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transform);
	}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);

	Destroy();
}

void ACItem_Arrow::Stucked()
{
	bStucked = true;
	StaticMesh->SetCollisionProfileName("Items");
	StaticMesh->SetSimulatePhysics(false);

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(30.0f);

	// effect ���
	// ���� ���
}

bool ACItem_Arrow::SphereTrace(const FVector& start, const FVector& end)
{
	bool result = false;

	TArray<AActor*> ignores;
	ignores.AddUnique(GetOwner());

	FHitResult h;

	result = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, 10.0f,
		ETraceTypeQuery::TraceTypeQuery1,
		false, ignores, EDrawDebugTrace::ForOneFrame, h, true);

	return result;
}

/*
	ȭ���� static mesh ��
	trail socket ���� ������

	���� trail particle�� �־�� ��

	�̰� inventory���� ������ ���� �߻�� ���Ŀ�,
	������ ����� ���̴� ����,
	Ȱ�� ������� �� �ؾ� ��

	Ȱ ���
	-> ��� �� ���õ� Ȱ�� ȭ���� ������ ��

	equip�� stackable �������� ����

	�� �༮�� �� ���� ȭ��
	 -> �����Ǹ� ���ư���,
	 ������ �ε����� ������ �ְ� �����,
	 ���� ������ ������

	 1. �κ��丮�� ȭ���� �����Ǿ���,
	 �ش� ������ 0�̻����� Ȯ���� �� �ִ� ����� �ʿ�
	 -> player ��
	 selected arrow �� �����ϸ�, �� ������ 0 �̻����� üũ

	 (enemy�� ���� �� ���� �˻縦 ���ص� ��)
	 (�ٸ� ȭ���� ������ Bow ��� ��ü�� �� �� �־�� ��)

	 -> ������ �κ��丮�� ������ ���� ��ü�� ��� ���̸�, Use �Լ��� �̿����� ����
	 (������ removeItem�� ������ �ʳ� ����)

	 -> ���� ActionBow ���� ȭ�� �߻� �ÿ�,
	 �ش� �������� removeItem ��Ű�� ���� ���ƺ���
	 (������ üũ�ؾ� �� -> �߻� ���θ�)

	 -> �߻� �ÿ� CItem_Arrow�� ������Ŵ
	 (��� ���� ����ϴ� �� ���߿� ���� ��ȭ�� ���� ���� Ÿ���� ȭ�� �ƴѰ�?)

	 �׳� consumable ������ �ϳ� ��� ������,
	 enum class ���η� �����ϴ� �� Ȯ���� ���ƺ��̱� �ϴµ�,
	 -> consumable item�̸�, �׳� use �� ó���ع����� �Ǵϱ�
	 -> ����Ʈ (��ƼŬ) � ���⼭ ������ �ǰ�


*/