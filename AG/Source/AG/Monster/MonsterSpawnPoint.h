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


	// 스폰시킬 몬스터 클래스.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMonster>	mSpawnClass;


	// 몬스터를 다시 스폰시키기 위한 시간.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpawnTime;


	// 이 스폰 포인트에서 몇 마리까지 생성할 수 있는가.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mSpawnCount;


	// 이 스폰 포인트에서 생성된 몬스터들을 저장.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonster*>	mMonstersArray;


	// 시간 체크용.
	float	mSpawnCheckTime;

};
