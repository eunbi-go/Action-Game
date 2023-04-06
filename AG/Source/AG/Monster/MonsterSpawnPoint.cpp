// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"

#include "Monster.h"
#include "Khaimera.h"
#include "AI/MonsterPatrolPoint.h"


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
	
	//--------------
	// PatrolPoint ��ġ�� �߰��Ѵ�.
	//--------------
	mPatrolPointPositionArray.Add(GetActorLocation());

	int32 count = mPatrolPointArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		mPatrolPointPositionArray.Add(mPatrolPointArray[i]->GetActorLocation());
	}



	//--------------
	// Monster�� ������Ų��.
	//--------------
	if (IsValid(mSpawnClass))
	{
		mSpawnCount = mSpawnCount < 1 ? 1 : mSpawnCount;


		FActorSpawnParameters spawnParam;
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector pos = GetActorLocation();

		AMonster* monster = GetWorld()->SpawnActor<AMonster>(mSpawnClass,
			GetActorLocation(),
			GetActorRotation(),
			spawnParam);

		monster->SetSpawnPoint(this);
		monster->SetPatrolPointPosition(mPatrolPointPositionArray);
		monster->SetPatrolDirection(mPatrolDirection);

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


			// ���� ����.
			FActorSpawnParameters spawnParam;
			spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// Type: AMonster. �θ� Ÿ������ Up Cast.
			// ���� �����Ǵ� ���� mSpawnClass�� ����ִ� ��.
			AMonster* monster = GetWorld()->SpawnActor<AMonster>(mSpawnClass,
				GetActorLocation(),
				GetActorRotation(),
				spawnParam);

			monster->SetSpawnPoint(this);
			monster->SetPatrolPointPosition(mPatrolPointPositionArray);
			monster->SetPatrolDirection(mPatrolDirection);

			mMonstersArray.Add(monster);
			

			// ���� ����Ʈ���� ������ ���Ͱ� ���� �� ����.
			// -> ���Ͱ� ������ �ٽ� ������Ų��.
			if (mMonstersArray.Num() == mSpawnCount)
			{
				mSpawnCheckTime = 0.f;
			}
		}
	}
}

