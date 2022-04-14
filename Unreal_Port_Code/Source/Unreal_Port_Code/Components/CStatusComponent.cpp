#include "CStatusComponent.h"

UCStatusComponent::UCStatusComponent()
{

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	FocusAmount = MaxFocusAmount;
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::AddFocus(float Amount)
{
	FocusAmount += Amount;
	FocusAmount = FMath::Clamp(FocusAmount, 0.0f, MaxFocusAmount);
}

void UCStatusComponent::SubFocus(float Amount)
{
	FocusAmount -= Amount;
	FocusAmount = FMath::Clamp(FocusAmount, 0.0f, MaxFocusAmount);
}
