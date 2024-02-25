// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"

AShield::AShield()
{
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(GetRootComponent());
}

void AShield::BeginPlay()
{
	Super::BeginPlay();
	mMesh->SetVisibility(false);
}

void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShield::Equip(USceneComponent* Parent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	AttachMeshToSocket(Parent, SocketName);
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
}

void AShield::SetSkeletalMesh(const FString& _path)
{
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetSkeletalMesh(mesh);
}

void AShield::AttachMeshToSocket(USceneComponent* _parent, const FName& _socketName)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	mMesh->AttachToComponent(_parent, transformRules, _socketName);
}