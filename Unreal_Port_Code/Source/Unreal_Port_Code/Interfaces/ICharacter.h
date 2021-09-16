#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class UNREAL_PORT_CODE_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void End_Dead() {}
	virtual void End_Hit() {}
	virtual void End_Stiff() {}
	virtual void End_Discover() {}
	virtual void End_Taunt() {}
	virtual void End_WakeUp() {}

	virtual	void RootMove() {}

public:
	virtual void LaunchByHitted() {}
	virtual void Popcorn() {}
	virtual void End_Popcorn() {}

public:
	virtual void Parrying(bool result) {}
	virtual void Guard(bool result) {}

};
