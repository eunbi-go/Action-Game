// Fill out your copyright notice in the Description page of Project Settings.


#include "Meteo.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerAnimInstance.h"
#include "../Monster/FengMao.h"
#include "../Basic/CollisionObject.h"

AMeteo::AMeteo()
{
	mParticle->SetGenerateOverlapEvents(true);
	mParticle->SetVisibility(true);
}

void AMeteo::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &AMeteo::Check, 0.5f, false);

	mParticle->OnSystemFinished.AddDynamic(this, &AMeteo::ParticleFinish);
}

void AMeteo::ParticleFinish(UNiagaraComponent* _particle)
{
	PrintViewport(3.f, FColor::Red, TEXT("AParticleNiagara::ParticleFinish"));
	mOnHittd.Clear();
	mCameraShake.Clear();
	Destroy();
}

void AMeteo::Check()
{
	mCameraShake.Broadcast(this);

	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector position = GetActorLocation();

	ACollisionObject* collObjg = GetWorld()->SpawnActor<ACollisionObject>(
		position,
		FRotator::ZeroRotator,
		params);
	collObjg->SetCollisionBoxExtent(FVector(120.0f));
	collObjg->mOnHit.AddDynamic(this, &AMeteo::Temp);
}

void AMeteo::Temp(ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor)
{
	mOnHittd.Broadcast(collisionObject, Hit, hitActor);
}
