// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//mMesh->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	//mMesh->SetNotifyRigidBodyCollision(true);
	//mMesh->SetSimulatePhysics(true);

	mBox->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	mBox->SetNotifyRigidBodyCollision(true);
	mBox->SetSimulatePhysics(true);
	
	SetRootComponent(mBox);

	mMesh->SetupAttachment(mBox);

	mIsHit = false;
	mDeadTime = 0.0f;
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	mBox->OnComponentHit.AddDynamic(this, &AItemActor::OnHit);

	mBox->SetBoxExtent(FVector(15.0f));
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mIsHit)
	{
		mDeadTime += DeltaTime;

		if (mDeadTime >= 0.5f)
			Destroy();
	}
}

void AItemActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PrintViewport(10.f, FColor::White, TEXT("coin death"));
	//Destroy();
	mIsHit = true;
}

void AItemActor::SetStaticMesh(const FString& _path)
{
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetStaticMesh(mesh);
}

