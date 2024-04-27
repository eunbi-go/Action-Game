// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionActor.h"
#include "../Interface/HitInterface.h"
#include "../Particle/ValkyrieLightning.h"
#include "../Particle/ValkyrieSlash.h"
#include "../Particle/ValkyrieDemonSlash.h"

ACollisionActor::ACollisionActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	mCollisionBox->SetupAttachment(mRoot);
	mCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	mCollisionCapsule->SetupAttachment(mRoot);
	mCollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	mCollisionSphere->SetupAttachment(mRoot);
	mCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mMoveDirection = FVector(0.f);
}

void ACollisionActor::BeginPlay()
{
	Super::BeginPlay();

	mCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnBoxOverlapBegin);
	mCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnBoxOverlapEnd);

	mCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnCapsuleOverlapBegin);
	mCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnCapsuleOverlapEnd);

	mCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnSphereOverlapBegin);
	mCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnBoxOverlapEnd);

	mActorsToIgnoreArray.Add(this);
}

void ACollisionActor::GetHit(AActor* _hitActor)
{
}

void ACollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollisionActor::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(1.f, FColor::White, TEXT("OnBoxOverlapBegin"));

	// test
	float damage = 10.f;

	if (OtherActor)
	{
		if (IsValid(mOwnerActor))
		{
			if (AParticleNiagara* niagara = Cast<AParticleNiagara>(mOwnerActor))
			{
				niagara->SpawnHitEffect(OtherActor->GetActorLocation(), FRotator::ZeroRotator);
			}
		}


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

					PrintViewport(1.f, FColor::Black, TEXT("GetHit"));
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
				0.9f,	// 몇 초 간격으로 피해를 줄 것인가. (test)
				true
				);
		}
	}

	Destroy();
}

void ACollisionActor::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ACollisionActor::OnCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ACollisionActor::OnCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ACollisionActor::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor);
	float damage = 10.f;
	
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
}

void ACollisionActor::ClearTimer()
{
	PrintViewport(1.f, FColor::Red, TEXT("finish"));
	GetWorldTimerManager().ClearTimer(mTimer);
	if (mCollisionBox)
	{
		mCollisionBox->DestroyComponent();
	}
	if (mCollisionCapsule)
		mCollisionCapsule->DestroyComponent();
	Destroy();
}
