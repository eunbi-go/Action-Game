// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleNiagara.h"

AParticleNiagara::AParticleNiagara()
{
	mParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));

	mParticle->SetupAttachment(mAudio);
}

void AParticleNiagara::BeginPlay()
{
	Super::BeginPlay();

	mParticle->OnSystemFinished.AddDynamic(this, &AParticleNiagara::ParticleFinish);
}

void AParticleNiagara::SetParticle(UNiagaraSystem* _particle)
{
	if (IsValid(_particle))
		mParticle->SetAsset(_particle);
}

void AParticleNiagara::SetParticle(const FString& _path)
{
	UNiagaraSystem* particle = LoadObject<UNiagaraSystem>(nullptr, *_path);

	if (IsValid(particle))
		mParticle->SetAsset(particle);
}

void AParticleNiagara::ParticleFinish(UNiagaraComponent* _particle)
{
	Destroy();
}
