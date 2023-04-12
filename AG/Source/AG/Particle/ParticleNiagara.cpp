// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleNiagara.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerAnimInstance.h"
#include "../Monster/FengMao.h"

AParticleNiagara::AParticleNiagara()
{
	mParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));

	
	mParticle->SetGenerateOverlapEvents(true);

	MyComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MyComp->BodyInstance.SetCollisionProfileName(TEXT("MonsterAttack"));
	MyComp->SetNotifyRigidBodyCollision(true);
	MyComp->SetBoxExtent(FVector(100.f));
	MyComp->SetActive(false);
	MyComp->SetSimulatePhysics(true);

	mParticle->SetupAttachment(mAudio);
	MyComp->SetupAttachment(mParticle);
	mParticle->SetVisibility(true);
}

void AParticleNiagara::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timerHandle, this, &AParticleNiagara::Check, 1.f, false);
	MyComp->OnComponentHit.AddDynamic(this, &AParticleNiagara::ParticleHit);
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
	mOnSHit.Clear();
	MyComp->DestroyComponent();
	Destroy();
}

void AParticleNiagara::ParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PrintViewport(1.f, FColor::Blue, TEXT("AParticleNiagara::ParticleHit()"));
	mOnSHit.Broadcast(this, Hit, OtherActor);
	MyComp->SetActive(false);
}

void AParticleNiagara::Check()
{
	// ÀÌ¶§ ¶³¾îÁü.
	MyComp->SetActive(true);
}
