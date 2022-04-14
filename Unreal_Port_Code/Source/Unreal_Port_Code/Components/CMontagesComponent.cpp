#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include <Animation/AnimMontage.h>

bool UCMontagesComponent::IsDataValid(EStateType state)
{
	for (UINT i = 0; i < 10; i++)
	{
		if (Datas[(int32)state][i] != nullptr)
		{
			return true;
		}
	}

	return false;
}

UCMontagesComponent::UCMontagesComponent()
{

}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	//GetRows from DataTable
	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	//Write Datas(Member) from TArray
	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		int32 j = 0;
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i][j] = data;

				j++;
				continue;
			}
		}
	}
}

void UCMontagesComponent::PlayEquip()
{
	PlayAnimMontage(EStateType::Equip);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayStiff()
{
	PlayAnimMontage(EStateType::Stiff);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayWakeUp()
{
	PlayAnimMontage(EStateType::WakeUp);
}

void UCMontagesComponent::PlayDodge()
{
	int32 index = 0;
	ACharacter* character = Cast<ACharacter>(GetOwner());
	CheckNull(character);

	float direction = Calculate(character->GetVelocity(), character->GetControlRotation());
	if (fabsf(direction) <= 45.0f) index = 0;							// forward
	else if (direction < -45.0f && direction >= -135.0f) index = 1;		// left
	else if (direction > 45.0f && direction <= 135.0f) index = 2;		// right
	else
	{
		index = 3;
	}

	PlayAnimMontage(EStateType::Dodge, index);
}

void UCMontagesComponent::PlayTaunt()
{
	PlayAnimMontage(EStateType::Taunt);
}

void UCMontagesComponent::PlayDiscover()
{
	PlayAnimMontage(EStateType::Discover);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType, int32 index)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	FMontageData* data = Datas[(int32)InStateType][index];
	if (!!data)
	{
		if (!!data->AnimMontage)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}

float UCMontagesComponent::Calculate(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = Velocity.GetSafeNormal2D();

		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.0f;
}
