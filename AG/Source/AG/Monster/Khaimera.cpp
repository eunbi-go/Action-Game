// Fill out your copyright notice in the Description page of Project Settings.


#include "Khaimera.h"

AKhaimera::AKhaimera()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);
	GetCapsuleComponent()->SetCapsuleRadius(30.f);
	

	mMonsterTableRowName = TEXT("Khaimera");
}

void AKhaimera::BeginPlay()
{
	Super::BeginPlay();
}

void AKhaimera::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AKhaimera::UnPossessed()
{
	Super::UnPossessed();
}
