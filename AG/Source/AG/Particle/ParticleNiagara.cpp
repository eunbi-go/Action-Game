// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleNiagara.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerAnimInstance.h"
#include "../Monster/FengMao.h"
#include "../Basic/CollisionObject.h"

AParticleNiagara::AParticleNiagara()
{
	mParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	mParticle->SetupAttachment(mAudio);
}

void AParticleNiagara::BeginPlay()
{
	Super::BeginPlay();

	mParticle->OnSystemFinished.AddDynamic(this, &AParticleNiagara::OnSystemFinish);
}

void AParticleNiagara::OnSystemFinish(UNiagaraComponent* PSystem)
{
	Destroy();
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

void AParticleNiagara::SetNiagaraScale(const FVector& _scale)
{
	if (mParticle)
		mParticle->SetWorldScale3D(_scale);
}

void AParticleNiagara::SpawnHitEffect(const FVector& Location, const FRotator& Rotator)
{
}
