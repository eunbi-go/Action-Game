// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActor.h"

// Sets default values
ASkillActor::ASkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	mNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));


	mSkeletalMesh->SetupAttachment(mRoot);
	mStaticMesh->SetupAttachment(mRoot);
	mParticle->SetupAttachment(mRoot);
	mNiagara->SetupAttachment(mRoot);



	mSound = nullptr;
	mRoot->bVisualizeComponent = true;
}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(mSound))
		UGameplayStatics::PlaySoundAtLocation(this, mSound, GetActorLocation());
}

void ASkillActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	mOnSkillEnd.Clear();
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActor::SetSkeletalMesh(const FString& _path)
{
	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(
		nullptr, *_path);

	if (IsValid(SkeletalMesh))
		mSkeletalMesh->SetSkeletalMesh(SkeletalMesh);
}

void ASkillActor::SetStaticMesh(const FString& _path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(
		nullptr, *_path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

void ASkillActor::SetParticle(const FString& _path)
{
	UParticleSystem* Particle = LoadObject<UParticleSystem>(
		nullptr, *_path);

	if (IsValid(Particle))
		mParticle->SetTemplate(Particle);
}

void ASkillActor::SetSound(const FString& _path)
{
	mSound = LoadObject<USoundBase>(nullptr, *_path);
}

void ASkillActor::SetNiagara(UNiagaraSystem* _niagara)
{
	if (IsValid(_niagara))
		mNiagara->SetAsset(_niagara);
}

void ASkillActor::SetNiagara(const FString& _path)
{
	UNiagaraSystem* Niagara = LoadObject<UNiagaraSystem>(
		nullptr, *_path);

	if (IsValid(Niagara))
		mNiagara->SetAsset(Niagara);
}

void ASkillActor::SetBoxExtent(const FVector& _extent)
{
	mRoot->SetBoxExtent(_extent);
}

void ASkillActor::SetCollisionProfile(const FName& _profile)
{
	mRoot->SetCollisionProfileName(_profile);
}
