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
	������ ������ ����
	�ʵ忡 �����Ǵ� ��ü

	����ƽ �޽ð� �־� �ʵ忡 ���� ����
	�÷��̾��� �ݴ� ���� ���� ���� �� UI�� �̸� ǥ��

	���� �ð��� ������ ������� �ؾ� ��

	�÷��̾��� �ݴ� �����ȿ� ������ �̸� ǥ�� -> �̰� �÷��̾ ��ȣ�� �����ش� ġ����,
	�׷��� ������ �̸� ��ǥ��					 -> �̰�?

	(�ݴ� ������ ����ٴ� ���� �˷��ִ� ����� �ֳ�?)
	(�׷��� ������ player�� Tick���� �˻��ؾ� ��)
	(��� 1. �÷��̾��� ���� �ȿ� �ִ� �༮�鿡�� Tick���� ��ȣ�� ��)
	(-> �ش� ��ȣ�� ������ 1�ʵ��� Widget�� �����ִٰ� ����)
	(Timer �̿� ���)

	(��� 2. �������� �� Tick ���� visible�� hidden���� ��)
	(�÷��̾ ���� ���� ���� �ش� tick�� �۵����� �ʰ� �ϸ�, visible�� ���̰� ����)


*/