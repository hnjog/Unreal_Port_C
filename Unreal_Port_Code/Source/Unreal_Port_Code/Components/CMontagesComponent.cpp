#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include <Animation/AnimMontage.h>

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
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;
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

void UCMontagesComponent::PlayTaunt()
{
	PlayAnimMontage(EStateType::Taunt);
}

void UCMontagesComponent::PlayDiscover()
{
	PlayAnimMontage(EStateType::Discover);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	CLog::Log(GetOwner()->GetName());

	FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}
