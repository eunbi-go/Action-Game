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
