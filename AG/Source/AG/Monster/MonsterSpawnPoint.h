// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

UCLASS()
class AG_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterSpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	void RemoveMonster(class AMonster* _monster) { mMonstersArray.Remove(_monster); }




protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* mRoot;


	// ������ų ���� Ŭ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMonster>	mSpawnClass;


	// ���͸� �ٽ� ������Ű�� ���� �ð�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpawnTime;


	// �� ���� ����Ʈ���� �� �������� ������ �� �ִ°�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mSpawnCount;


	// �� ���� ����Ʈ���� ������ ���͵��� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonster*>	mMonstersArray;


	// �ð� üũ��.
	float	mSpawnCheckTime;

};
