// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "../Player/PlayerCharacter.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	mBox->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	mBox->SetNotifyRigidBodyCollision(true);
	mBox->SetSimulatePhysics(true);
	
	SetRootComponent(mBox);

	mMesh->SetupAttachment(mBox);
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
}

void AItemActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	mOnHitt.Broadcast(this, Hit, OtherActor);
}

void AItemActor::SetStaticMesh(const FString& _path)
{
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetStaticMesh(mesh);
}

