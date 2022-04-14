#include "CAblityComponent.h"
#include"Global.h"
#include"GameFramework/Character.h"
#include"Interfaces/ITargetAble.h"
#include "Components/SplineComponent.h"

UCAblityComponent::UCAblityComponent()
{

}

void UCAblityComponent::TimeFreeze()
{
	CTimeDilationManager::SetGlobalTime(GetWorld(), WorldTimeRation);
	GetOwner()->CustomTimeDilation = PlayerTimeRatio;
	UKismetSystemLibrary::K2_SetTimer(this, "ResetTimeDilation", StopTime * WorldTimeRation, false);
	bTimeFreeze = true;
}

void UCAblityComponent::ResetTimeDilation()
{
	CTimeDilationManager::SetGlobalTime(GetWorld(), WorldTimeRation, true);
	GetOwner()->CustomTimeDilation = 1.0f;
	bTimeFreeze = false;

	if (OnTimeFreezeEnd.IsBound())
		OnTimeFreezeEnd.Broadcast();
}

void UCAblityComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCAblityComponent::SetTarget()
{
	TraceTargets();

	float angle = 0.0f;
	for (IITargetAble* target : Targets)
	{
		AActor* actor = Cast<AActor>(target);
		if (!!actor)
		{
			if (angle <= GetOwner()->GetDotProductTo(actor))
			{
				angle = GetOwner()->GetDotProductTo(actor);
				Target = target;
			}
		}
	}

	if (!!Target)
	{
		bTargeting = true;
		Target->TargetWidgetVisible(true);
	}
	else
	{
		bTargeting = false;
		Target = nullptr;
	}

}

void UCAblityComponent::EndTimeFreeze()
{
	ResetTimeDilation();
}

void UCAblityComponent::ClearTargets()
{
	for (IITargetAble* target : Targets)
	{
		target->TargetWidgetVisible(false);
	}

	bTargeting = false;
	Targets.Empty();
	Target = nullptr;
}

bool UCAblityComponent::IsTargetStiff()
{
	if (Target == nullptr || Target->IsTargetStiffed() == false)
		return false;

	return true;
}

bool UCAblityComponent::IsTargetKnoukout()
{
	if (Target == nullptr || Target->IsTargetKnockOuted() == false)
		return false;

	return true;
}

bool UCAblityComponent::IsTargetFalling()
{
	if (Target == nullptr) return false;

	return  Target->IsTargetFalling();
}

void UCAblityComponent::TraceTargets()
{
	TArray<AActor*> ignores;
	ignores.Add(GetOwner());
	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMulti(
		GetOwner()->GetWorld(),
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation() + FVector(1, 0, 0),
		LookOnDistance,
		ETraceTypeQuery::TraceTypeQuery1,	// Trace 유형의 1번 (Visiblity)
		false,
		ignores,
		EDrawDebugTrace::ForOneFrame,
		hitResults,
		true
	);

	for (const FHitResult& hitResult : hitResults)
	{
		// root 가 UStaticMeshComponent 인 녀석들로 가리기 위하여
		IITargetAble* target = Cast<IITargetAble>(hitResult.GetActor());
		if (!!target)
			Targets.AddUnique(target);
	}

}

void UCAblityComponent::TargetBeaten()
{
	CheckNull(Target);
	Target->TargetBeaten();
}

void UCAblityComponent::EndTargetBeaten()
{
	CheckNull(Target);
	Target->End_TargetBeaten();
}