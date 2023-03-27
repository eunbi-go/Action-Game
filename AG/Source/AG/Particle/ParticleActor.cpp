// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleActor.h"

// Sets default values
AParticleActor::AParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

//void AParticleActor::SetParticle(UNiagaraSystem* _particle)
//{
//}

void AParticleActor::SetParticle(const FString& _path)
{
}

