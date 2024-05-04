// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleActor.h"

AParticleActor::AParticleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	mAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	SetRootComponent(mAudio);
}

void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParticleActor::SpawnHitEffect(const FVector& Location, const FRotator& Rotator)
{
}

void AParticleActor::SetSound(const FString& _path, bool _isPlay)
{
	USoundBase* Sound = LoadObject<USoundBase>(
		nullptr, *_path);

	if (IsValid(Sound))
	{
		mAudio->SetSound(Sound);

		if (_isPlay)
			mAudio->Play();
	}
}

void AParticleActor::SetSound(USoundBase* _sound, bool _isPlay)
{
	if (IsValid(_sound))
	{
		mAudio->SetSound(_sound);

		if (_isPlay)
			mAudio->Play();
	}
}

void AParticleActor::SetParticle(UParticleSystem* _particle)
{
}

void AParticleActor::SetParticle(UNiagaraSystem* _particle)
{
}

void AParticleActor::SetParticle(const FString& _path)
{
}

