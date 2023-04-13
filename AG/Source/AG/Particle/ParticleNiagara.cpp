// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleNiagara.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerAnimInstance.h"
#include "../Monster/FengMao.h"
#include "../Basic/CollisionObject.h"

AParticleNiagara::AParticleNiagara()
{
	mParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	
	mParticle->SetGenerateOverlapEvents(true);

	mParticle->SetupAttachment(mAudio);
	mParticle->SetVisibility(true);
}

void AParticleNiagara::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &AParticleNiagara::Check, 0.5f, false);
	
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
	PrintViewport(3.f, FColor::Red, TEXT("AParticleNiagara::ParticleFinish"));
	mOnHittd.Clear();
	mCameraShake.Clear();
	Destroy();
}

void AParticleNiagara::Check()
{
	mCameraShake.Broadcast(this);

	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector position = GetActorLocation();

	ACollisionObject* collObjg = GetWorld()->SpawnActor<ACollisionObject>(
		position,
		FRotator::ZeroRotator,
		params);

	collObjg->mOnHitt.AddDynamic(this, &AParticleNiagara::Temp);
}

void AParticleNiagara::Temp(ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor)
{
	mOnHittd.Broadcast(collisionObject, Hit, hitActor);
}
