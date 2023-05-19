// Fill out your copyright notice in the Description page of Project Settings.


#include "FresnelActor.h"

// Sets default values
AFresnelActor::AFresnelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mPoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Mesh"));
	
	SetRootComponent(mPoseableMesh);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> material(TEXT("MaterialInstanceConstant'/Game/MT_Fresnel_Inst.MT_Fresnel_Inst'"));

	if (material.Succeeded())
		mFresnelMaterial = material.Object;

	mLifeTime = 1.f;
	mFadeTime = 0.f;
	mFadeTimeMax = 1.f;

	mIsFade = false;
}

// Called when the game starts or when spawned
void AFresnelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFresnelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	mLifeTime -= DeltaTime;

	if (mLifeTime <= 0.f)
	{
		if (mIsFade)
		{
			mFadeTime += DeltaTime;
			float opacity = 1.f - mFadeTime / mFadeTimeMax;

			for (auto& material : mMaterialArray)
			{
				material->SetScalarParameterValue(TEXT("Opacity"), opacity);
			}

			if (mFadeTime >= mFadeTimeMax)
				Destroy();
		}
		else
			Destroy();

	}
}

void AFresnelActor::SetMesh(const FString& _path)
{
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(GetWorld(), *_path);

	SetMesh(mesh);
}

void AFresnelActor::SetMesh(USkeletalMesh* _mesh)
{
	if (!IsValid(_mesh))
		return;

	mPoseableMesh->SetSkeletalMesh(_mesh);

	if (mFresnelMaterial)
	{
		int32	count = mPoseableMesh->GetNumMaterials();

		for (int32 i = 0; i < count; ++i)
		{
			UMaterialInstanceDynamic* material =
				mPoseableMesh->CreateDynamicMaterialInstance(i, mFresnelMaterial);
			if (!IsValid(material))
			{
			}
			mMaterialArray.Add(material);
		}
	}
}

void AFresnelActor::CopyPoseFromSkeletalComp(USkeletalMeshComponent* _skeletalMesh)
{
	mPoseableMesh->CopyPoseFromSkeletalComponent(_skeletalMesh);
}

void AFresnelActor::SetFresnelMaterial(const FString& _path)
{
	UMaterialInterface* material = LoadObject<UMaterialInterface>(GetWorld(), *_path);

	SetFresnelMaterial(material);
}

void AFresnelActor::SetFresnelMaterial(UMaterialInterface* _material)
{
	if (!IsValid(_material))
		return;

	mFresnelMaterial = _material;
}

