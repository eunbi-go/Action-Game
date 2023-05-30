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

public:
	virtual void OnConstruction(const FTransform& Transform);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void ComputeSpline();

public:
	void RemoveMonster(class AMonster* _monster)
	{ 
		mMonstersArray.Remove(_monster); 
	}


public:
	float GetSplineLength() const 
	{ 
		return mSplineLength; 
	}

	const TArray<FVector>& GetSplinePositionArray() const 
	{
		return mSplinePosition; 
	}

	FVector GetSplinePosition(int32 _index) const 
	{
		return mSplinePosition[_index]; 
	}

	FVector GetSplinePosition(float _cellDistance) 
	{
		return mPatrolSpline->GetLocationAtDistanceAlongSpline(_cellDistance, ESplineCoordinateSpace::World); 
	}




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


protected:
	//--------------------
	// Patrol 관련 변수들: POINT TYPE.
	//--------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonsterPatrolPoint*>	mPatrolPointArray;

	TArray<FVector>	mPatrolPointPositionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	PATROL_END_DIRECTION	mPatrolDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	PATROL_TYPE	mPatrolType;



	//--------------------
	// Patrol 관련 변수들: SPLINE TYPE.
	//--------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USplineComponent* mPatrolSpline;

	// 곡선을 몇 등분으로 나눌 것인지.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mDivideCount;

	// 나눠진 위치마다 메시를 출력할 것인지.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsDivideMeshVisible;

	// 나눠진 위치마다 출력할 메시.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh* mDivideMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*>	mMeshArray;

	// 나눠진 위치 정보.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FVector>	mSplinePosition;

	// 나눠진 위치 회전 정보.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FRotator>	mSplineRotation;

	// 스플라인 길이.
	float	mSplineLength;

	// 한 칸 거리.
	float	mCellDistance;

	// 이전에 나눈 개수.
	int32	mPreDivideCount;

	// 이전 스플라인 길이.
	float	mPreLength;
};
