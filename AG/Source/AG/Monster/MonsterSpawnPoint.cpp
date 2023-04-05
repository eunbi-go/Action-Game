// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "Monster.h"
#include "Khaimera.h"
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mRoot->bVisualizeComponent = true;

	mSpawnCheckTime = 0.f;
	mSpawnTime = 3.f;
	mSpawnCount = 1;

	mSpawnClass = AKhaimera::StaticClass();

}

void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(mSpawnClass))
	{

		mSpawnCount = mSpawnCount < 1 ? 1 : mSpawnCount;


		// 시작할 때 한 마리는 무조건 스폰시킴.
		FActorSpawnParameters spawnParam;
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Type: AMonster. 부모 타입으로 Up Cast.
		// 실제 생성되는 것은 mSpawnClass에 들어있는 것.
		AMonster* monster = GetWorld()->SpawnActor<AMonster>(mSpawnClass,
			GetActorLocation(),
			GetActorRotation(),
			spawnParam);

		monster->SetSpawnPoint(this);

		mMonstersArray.Add(monster);
	}
}

void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(mSpawnClass))
		return;

	if (mMonstersArray.Num() < mSpawnCount)
	{
		mSpawnCheckTime += DeltaTime;

		if (mSpawnCheckTime >= mSpawnTime)
		{
			mSpawnCheckTime -= mSpawnCheckTime;


			// 몬스터 스폰.
			FActorSpawnParameters spawnParam;
			spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// Type: AMonster. 부모 타입으로 Up Cast.
			// 실제 생성되는 것은 mSpawnClass에 들어있는 것.
			AMonster* monster = GetWorld()->SpawnActor<AMonster>(mSpawnClass,
				GetActorLocation(),
				GetActorRotation(),
				spawnParam);

			monster->SetSpawnPoint(this);

			mMonstersArray.Add(monster);
			

			// 스폰 포인트에서 스폰된 몬스터가 죽을 수 있음.
			// -> 몬스터가 죽으면 다시 스폰시킨다.
			if (mMonstersArray.Num() == mSpawnCount)
			{
				mSpawnCheckTime = 0.f;
			}
		}
	}
}

