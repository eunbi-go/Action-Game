// Fill out your copyright notice in the Description page of Project Settings.


#include "Decal.h"

// Sets default values
ADecal::ADecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));

	SetRootComponent(mRoot);

	mDecal->SetupAttachment(mRoot);

	mRoot->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	//mRoot->bVisualizeComponent = true;
}

// Called when the game starts or when spawned
void ADecal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADecal::SetDecalMaterial(const FString& Path)
{
	UMaterialInterface* Mtrl = LoadObject<UMaterialInterface>(nullptr, *Path);

	if (IsValid(Mtrl))
		mDecal->SetDecalMaterial(Mtrl);
}

