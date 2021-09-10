#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemFiled.generated.h"

UCLASS()
class UNREAL_PORT_CODE_API ACItemFiled : public AActor
{
	GENERATED_BODY()

public:
	ACItemFiled();

	void SetItem(class UCItem* item);

	FORCEINLINE class UCItem* GetItem() { return Item; }

	void OnVisible();
	void UnActive();

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* StaticMesh;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
		class UCItem* Item;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

};

/*
	아이템 정보를 담은
	필드에 생성되는 객체

	스태틱 메시가 있어 필드에 존재 가능
	플레이어의 줍는 범위 내에 있을 시 UI로 이름 표시

	일정 시간이 지나면 사라지게 해야 함

	플레이어의 줍는 범위안에 들어오면 이름 표시 -> 이건 플레이어가 신호를 보내준다 치지만,
	그렇지 않으면 이름 비표시					 -> 이건?

	(줍는 범위를 벗어났다는 것을 알려주는 기능이 있나?)
	(그렇지 않으면 player의 Tick에서 검사해야 함)
	(방법 1. 플레이어의 범위 안에 있는 녀석들에게 Tick으로 신호를 쏨)
	(-> 해당 신호를 받으면 1초동안 Widget을 보여주다가 꺼줌)
	(Timer 이용 방식)

	(방법 2. 아이템은 매 Tick 마다 visible을 hidden으로 함)
	(플레이어가 보고 있을 때는 해당 tick을 작동하지 않게 하며, visible을 보이게 설정)


*/