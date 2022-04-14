#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAblityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimeFreezeEnd);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PORT_CODE_API UCAblityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAblityComponent();

private:
	UPROPERTY(EditDefaultsOnly, Category = "TimeFreeze")
		float WorldTimeRation = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "TimeFreeze")
		float PlayerTimeRatio = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TimeFreeze")
		float StopTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
		float LookOnDistance = 3000.0f;

public:
	// Ablity
	UFUNCTION(BlueprintCallable)
		void TimeFreeze();
	
	void SetTarget();
	void EndTimeFreeze();

	void TargetBeaten();
	void EndTargetBeaten();

	// Information
	FORCEINLINE class IITargetAble* GetTarget() { return Target; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE AActor* GetTargetActor() { return Cast<AActor>(Target); }
	
	void ClearTargets();
	FORCEINLINE float GetDistance() { return LookOnDistance; }

	FORCEINLINE bool IsTimeFreeze() { return bTimeFreeze; }
	FORCEINLINE bool IsTargeting() { return bTargeting; }
	bool IsTargetStiff();
	bool IsTargetKnoukout();
	bool IsTargetFalling();

public:
	UPROPERTY(BlueprintAssignable)
		FTimeFreezeEnd OnTimeFreezeEnd;

private:
	UFUNCTION()
		void ResetTimeDilation();

	void TraceTargets();

protected:
	virtual void BeginPlay() override;

private:
	bool bTimeFreeze = false;
	bool bTargeting = false;

	//For Target
	TArray<class IITargetAble*> Targets;

	class IITargetAble* Target;
};

/*
	global Time dilation과
	custom Time Dilation의 곱이
	해당 Actor의 시간이 흐르는 속도가 됨

	타겟 기능
	대상 타겟이 stiff 상태인지,
	혹은 knockOut 상태인지도 반환 가능하도록
*/