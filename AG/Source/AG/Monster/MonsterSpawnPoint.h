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


protected:
	//--------------------
	// Patrol ���� ������: POINT TYPE.
	//--------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonsterPatrolPoint*>	mPatrolPointArray;

	TArray<FVector>	mPatrolPointPositionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	PATROL_END_DIRECTION	mPatrolDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	PATROL_TYPE	mPatrolType;



	//--------------------
	// Patrol ���� ������: SPLINE TYPE.
	//--------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USplineComponent* mPatrolSpline;

	// ��� �� ������� ���� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mDivideCount;

	// ������ ��ġ���� �޽ø� ����� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsDivideMeshVisible;

	// ������ ��ġ���� ����� �޽�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh* mDivideMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*>	mMeshArray;

	// ������ ��ġ ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FVector>	mSplinePosition;

	// ������ ��ġ ȸ�� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FRotator>	mSplineRotation;

	// ���ö��� ����.
	float	mSplineLength;

	// �� ĭ �Ÿ�.
	float	mCellDistance;

	// ������ ���� ����.
	int32	mPreDivideCount;

	// ���� ���ö��� ����.
	float	mPreLength;
};
