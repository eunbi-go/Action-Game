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
			monster->SetPatrolType(mPatrolType);
			monster->SetPatrolSplineLength(mSplineLength);
			monster->SetPatrolCellDistance(mCellDistance);
			monster->SetPatrolSplineCount(mDivideCount);

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

void AMonsterSpawnPoint::ComputeSpline()
{
	//----------------------
	// ������ �޽ø� ����ؾ� �ϰ�, ���� CP�� ���� CP�� �ٸ���.
	// -> ���� �޽� ������Ʈ, mMeshArray ��� ����.
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
	// �޽� ����� �ʿ䰡 ������ ���� �޽� ������Ʈ, mMeshArray ��� �����Ѵ�.
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
		// ���ö��� CP������ ��ġ, ȸ�� ������ ���´�.
		//----------------------
	
		FVector position = mPatrolSpline->GetLocationAtDistanceAlongSpline(
			mCellDistance * i, ESplineCoordinateSpace::World);

		mSplinePosition.Add(position);

		FVector localPosition = transform.InverseTransformPosition(position);

		FRotator rotation = mPatrolSpline->GetRotationAtDistanceAlongSpline(
			mCellDistance * i, ESplineCoordinateSpace::World);

		mSplineRotation.Add(rotation);


		//----------------------
		// ���ö��� CP �� �޽ø� ����ؾ� �Ѵٸ� �޽ø� �������ش�.
		//----------------------

		if (mIsDivideMeshVisible)
		{
			// CP �� ������ ������ �ٸ��� �޽ø� ���� �����ؼ� �����Ѵ�.
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
				// ���⼭ ������.
				mMeshArray[i]->SetRelativeLocation(localPosition);
				mMeshArray[i]->SetWorldRotation(rotation);
			}
		}
	}

	mPreDivideCount = mDivideCount;
	mPreLength = mSplineLength;
}

