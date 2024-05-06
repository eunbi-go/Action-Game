// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionActor.h"
#include "../Interface/HitInterface.h"
#include "../Particle/ValkyrieSlash.h"
#include "../Particle/ParticleCascade.h"

ACollisionActor::ACollisionActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mHitType = EHitType::EHT_Once;

	mCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	mCollisionBox->SetupAttachment(mRoot);
	mCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	mCollisionCapsule->SetupAttachment(mRoot);
	mCollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	mCollisionSphere->SetupAttachment(mRoot);
	mCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ACollisionActor::BeginPlay()
{
	Super::BeginPlay();

	mCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnOverlapBegin);
	mCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnOverlapEnd);

	mCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnOverlapBegin);
	mCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnOverlapEnd);

	mCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnOverlapBegin);
	mCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnOverlapEnd);
}

void ACollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollisionActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(3.f, FColor::Blue, TEXT("OnBoxOverlapBegin"));

	// test
	float damage = 10.f;

	if (IsValid(mOwnerActor))
	{
		if (AParticleNiagara* niagara = Cast<AParticleNiagara>(mOwnerActor))
		{
			niagara->SpawnHitEffect(OtherActor->GetActorLocation(), FRotator::ZeroRotator);
		}
		else if (AParticleCascade* cascade = Cast<AParticleCascade>(mOwnerActor))
		{
			cascade->SpawnHitEffect(OtherActor->GetActorLocation(), FRotator::ZeroRotator);
		}
	}


	if (OtherActor)
	{
		IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor);

		if (mHitType == EHitType::EHT_Once)
		{
			if (hitInterface)
			{
				hitInterface->GetHit(SweepResult.ImpactPoint);
			}
			OtherActor->TakeDamage(
				damage,
				FDamageEvent(),
				GetWorld()->GetFirstPlayerController(),
				this);
		}

		else if (mHitType == EHitType::EHT_Continuous)
		{
			GetWorld()->GetTimerManager().SetTimer(
				mTimer,
				FTimerDelegate::CreateLambda([this, SweepResult, damage, hitInterface, OtherActor]() {
					PrintViewport(1.f, FColor::Green, FString("hit"));
					if (hitInterface)
					{
						hitInterface->GetHit(SweepResult.ImpactPoint);
					}
					OtherActor->TakeDamage(
						damage,
						FDamageEvent(),
						GetWorld()->GetFirstPlayerController(),
						this);

				}),
				mHitSeconds,	
				true
				);
		}
	}

	Destroy();
}

void ACollisionActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ACollisionActor::ClearTimer()
{
	GetWorldTimerManager().ClearTimer(mTimer);
	if (mCollisionBox)
		mCollisionBox->DestroyComponent();
	if (mCollisionCapsule)
		mCollisionCapsule->DestroyComponent();
	if (mCollisionSphere)
		mCollisionSphere->DestroyComponent();
	Destroy();
}
