// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"

#include "Monster.h"
#include "Khaimera.h"
#include "AI/MonsterPatrolPoint.h"


AMonsterSpawnPoint::AMonsterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mPatrolSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolSpline"));

	SetRootComponent(mRoot);
	mPatrolSpline->SetupAttachment(mRoot);

	mRoot->bVisualizeComponent = true;

	mSpawnCheckTime = 0.f;
	mSpawnTime = 3.f;
	mSpawnCount = 1;

	mDivideCount = 5;
	mIsDivideMeshVisible = true;
	mSplineLength = 0.f;
	mCellDistance = 0.f;
	mPreDivideCount = 0;
	mPreLength = 0.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	DivideMesh(TEXT("StaticMesh'/Game/Assets/ParagonGreystone/FX/Meshes/Heroes/Greystone/Skins/Novaborn/SM_ParticleCube.SM_ParticleCube'"));

	if (DivideMesh.Succeeded())
		mDivideMesh = DivideMesh.Object;

	mPatrolType = PATROL_TYPE::SPLINE;

	mSpawnClass = AKhaimera::StaticClass();
}

void AMonsterSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ComputeSpline();
}

void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	mSplineLength = mPatrolSpline->GetSplineLength();
	mCellDistance = mSplineLength / mDivideCount;

	//--------------
	// PatrolPoint 위치를 추가한다.
	//--------------
	mPatrolPointPositionArray.Add(GetActorLocation());

	int32 count = mPatrolPointArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		mPatrolPointPositionArray.Add(mPatrolPointArray[i]->GetActorLocation());
	}



	//--------------
	// Monster를 스폰시킨다.
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
		monster->SetPatrolType(mPatrolType);
		monster->SetPatrolSplineLength(mSplineLength);
		monster->SetPatrolCellDistance(mCellDistance);
		monster->SetPatrolSplineCount(mDivideCount);

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
			monster->SetPatrolPointPosition(mPatrolPointPositionArray);
			monster->SetPatrolDirection(mPatrolDirection);
			monster->SetPatrolType(mPatrolType);
			monster->SetPatrolSplineLength(mSplineLength);
			monster->SetPatrolCellDistance(mCellDistance);
			monster->SetPatrolSplineCount(mDivideCount);

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

void AMonsterSpawnPoint::ComputeSpline()
{
	//----------------------
	// 나눠질 메시를 출력해야 하고, 지금 CP와 이전 CP가 다르다.
	// -> 기존 메시 컴포넌트, mMeshArray 모두 삭제.
	//----------------------

	if (mIsDivideMeshVisible && mDivideCount != mPreDivideCount)
	{
		int32 count = mMeshArray.Num();

		for (int32 i = 0; i < count; ++i)
		{
			if (mMeshArray[i])
				mMeshArray[i]->DestroyComponent();
		}

		if (mMeshArray.Num() > 0)
			mMeshArray.RemoveAll([](UStaticMeshComponent*) {return true; });
	}

	//----------------------
	// 메시 출력할 필요가 없으면 기존 메시 컴포넌트, mMeshArray 모두 삭제한다.
	//----------------------

	else
	{
		int32 count = mMeshArray.Num();

		for (int32 i = 0; i < count; ++i)
		{
			if (mMeshArray[i])
				mMeshArray[i]->DestroyComponent();
		}

		if (mMeshArray.Num() > 0)
			mMeshArray.RemoveAll([](UStaticMeshComponent*) {return true; });
	}



	mSplinePosition.RemoveAll([](FVector) {return true; });
	mSplineRotation.RemoveAll([](FRotator) {return true; });

	mSplineLength = mPatrolSpline->GetSplineLength();
	mCellDistance = mSplineLength / mDivideCount;



	FTransform transform = GetActorTransform();

	for (int32 i = 0; i < mDivideCount; ++i)
	{
		//----------------------
		// 스플라인 CP에서의 위치, 회전 정보를 얻어온다.
		//----------------------
	
		FVector position = mPatrolSpline->GetLocationAtDistanceAlongSpline(
			mCellDistance * i, ESplineCoordinateSpace::World);

		mSplinePosition.Add(position);

		FVector localPosition = transform.InverseTransformPosition(position);

		FRotator rotation = mPatrolSpline->GetRotationAtDistanceAlongSpline(
			mCellDistance * i, ESplineCoordinateSpace::World);

		mSplineRotation.Add(rotation);


		//----------------------
		// 스플라인 CP 에 메시를 출력해야 한다면 메시를 세팅해준다.
		//----------------------

		if (mIsDivideMeshVisible)
		{
			// CP 의 개수가 이전과 다르면 메시를 새로 생성해서 세팅한다.
			if (mDivideCount != mPreDivideCount)
			{
				UStaticMeshComponent* staticMeshComp = NewObject<UStaticMeshComponent>(this);

				FTransform staticMeshTransform = staticMeshComp->GetRelativeTransform();

				staticMeshTransform.SetLocation(localPosition);
				staticMeshTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));

				staticMeshComp->SetRelativeTransform(staticMeshTransform);
				staticMeshComp->SetWorldRotation(rotation);

				staticMeshComp->SetStaticMesh(mDivideMesh);

				staticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				mMeshArray.Add(staticMeshComp);

				staticMeshComp->SetupAttachment(mPatrolSpline);
			}

			else
			{
				// 여기서 오류남.
				mMeshArray[i]->SetRelativeLocation(localPosition);
				mMeshArray[i]->SetWorldRotation(rotation);
			}
		}
	}

	mPreDivideCount = mDivideCount;
	mPreLength = mSplineLength;
}

