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

	// Initial speed 와 velocity
	// 0이상이면 초기 velocity값을 무시하고 velocity를 방향으로 처리함

	InitialLifeSpan = 10.0f;

}

void ACItem_Arrow::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACItem_Arrow::OnComponentBeginOverlap);

	// nocollision으로 해야 잘 나감
	// 이제 어딘가에 부딪히면 정지시키는 역할이 필요함
	// + 그 경우 StaticMesh가 Items으로 바뀌고, Simulate 가 false로 바뀌어 고정되어야 하겠지
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetSimulatePhysics(false);

	//Item->Count = 1;
}

void ACItem_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 결국 tick에서 sphere trace를 해야 할듯?

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
	CheckTrue(bStucked);				// bStucked 가 true이면 이미 어딘가에 박혀있다는 뜻

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

	// effect 재생
	// 사운드 재생
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
	화살의 static mesh 중
	trail socket 들을 가져와

	관련 trail particle을 넣어야 함

	이걸 inventory에서 꺼내는 것은 발사된 이후에,
	하지만 모습이 보이는 것은,
	활을 장비했을 때 해야 함

	활 장비
	-> 장비 및 선택된 활과 화살이 보여야 함

	equip에 stackable 버전으로 만듦

	이 녀석은 쏠 때의 화살
	 -> 생성되면 날아가며,
	 적에게 부딪히면 데미지 주고 사라짐,
	 벽에 맞으면 고정됨

	 1. 인벤토리에 화살이 장착되었고,
	 해당 개수가 0이상인지 확인할 수 있는 방법이 필요
	 -> player 에
	 selected arrow 가 존재하며, 그 개수가 0 이상인지 체크

	 (enemy의 경우는 이 조건 검사를 안해도 됨)
	 (다만 화살이 없더라도 Bow 모드 자체는 할 수 있어야 함)

	 -> 어차피 인벤토리의 아이템 개수 자체를 깎는 것이며, Use 함수는 이용하지 않음
	 (오히려 removeItem에 가깝지 않나 싶음)

	 -> 따라서 ActionBow 에서 화살 발사 시에,
	 해당 아이템을 removeItem 시키는 것이 좋아보임
	 (개수는 체크해야 함 -> 발사 여부만)

	 -> 발사 시에 CItem_Arrow를 생성시킴
	 (사실 지금 고민하는 거 나중에 넣을 불화살 같은 여러 타입의 화살 아닌가?)

	 그냥 consumable 아이템 하나 상속 받은뒤,
	 enum class 내부로 정리하는 게 확실히 좋아보이긴 하는데,
	 -> consumable item이면, 그냥 use 로 처리해버리면 되니까
	 -> 이펙트 (파티클) 등도 여기서 받으면 되고


*/