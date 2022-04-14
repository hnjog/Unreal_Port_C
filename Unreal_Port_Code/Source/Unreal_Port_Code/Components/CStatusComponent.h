#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PORT_CODE_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCStatusComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float CrouchSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Focus")
		float MaxFocusAmount = 5.0f;

public:
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxFocusAmount() const { return MaxFocusAmount; }
	FORCEINLINE float GetFocusAmount() const { return FocusAmount; }

	FORCEINLINE float GetCrouchSpeed() const { return CrouchSpeed; }
	FORCEINLINE float GetRunSpeed() const { return RunSpeed; }
	FORCEINLINE float GetSprintSpeed() const { return SprintSpeed; }

	FORCEINLINE bool CanMove() const { return bCanMove; }
	FORCEINLINE bool CanFocus() const { return FocusAmount >= 1.0f; }
	FORCEINLINE bool NotFullFocus() const { return FocusAmount < MaxFocusAmount; }
	

public:
	void SetMove();
	void SetStop();

	UFUNCTION(BlueprintCallable)
		void AddHealth(float InAmount);

	void SubHealth(float InAmount);

	void AddFocus(float Amount);
	void SubFocus(float Amount);

protected:
	virtual void BeginPlay() override;

private:
	bool bCanMove = true;
	float Health;
	float FocusAmount;

};
