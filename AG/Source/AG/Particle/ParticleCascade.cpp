// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleCascade.h"

AParticleCascade::AParticleCascade()
{
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	mParticle->SetupAttachment(mAudio);
}

void AParticleCascade::BeginPlay()
{
	Super::BeginPlay();

	mParticle->OnSystemFinished.AddDynamic(this, &AParticleCascade::ParticleFinish);
	//mParticle->OnParticleCollide.AddDynamic(this, &AParticleCascade::ParticleColiision);
}

void AParticleCascade::SetParticle(UParticleSystem* _particle)
{
	if (IsValid(_particle))
		mParticle->SetTemplate(_particle);
}

void AParticleCascade::SetParticle(const FString& _path)
{
	UParticleSystem* particle = LoadObject<UParticleSystem>(nullptr, *_path);

	if (IsValid(particle))
		mParticle->SetTemplate(particle);
}

void AParticleCascade::ParticleFinish(UParticleSystemComponent* _particle)
{
	PrintViewport(10.f, FColor::Yellow, TEXT("AParticleCascade::ParticleFinish()"));
	Destroy();
}

void AParticleCascade::ParticleColiision(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat)
{
}
