// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleNiagara.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/PlayerAnimInstance.h"

AParticleNiagara::AParticleNiagara()
{
	mParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));

	mParticle->SetupAttachment(mAudio);
	mParticle->SetGenerateOverlapEvents(true);
	mParticle->SetSimulatePhysics(true);

	MyComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MyComp->SetupAttachment(mParticle);
	//MyComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	mParticle->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//MyComp->OnComponentHit.AddDynamic(this, &AParticleNiagara::ParticleHit);
	MyComp->SetNotifyRigidBodyCollision(true);
	MyComp->SetBoxExtent(FVector(70.f));

	MyComp->bDynamicObstacle = true;
}

void AParticleNiagara::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timerHandle, this, &AParticleNiagara::Check, 0.5f, false);
	mParticle->OnComponentHit.AddDynamic(this, &AParticleNiagara::ParticleHit);
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
	MyComp->DestroyComponent();
	Destroy();
}

void AParticleNiagara::ParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
		mHitResult = Hit;
		mIsHit = true;
		PrintViewport(1.f, FColor::Blue, TEXT("AParticleNiagara::ParticleHit()"));
}

void AParticleNiagara::Check()
{
	// ÀÌ¶§ ¶³¾îÁü.

	if (mIsHit)
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(mHitResult.GetActor());
		if (IsValid(player))
		{
			player->GetAnimInstance()->SetHitDirection(TEXT("Front"));
			PrintViewport(1.f, FColor::Blue, TEXT("AParticleNiagara::Check()"));

		}


	}
}
