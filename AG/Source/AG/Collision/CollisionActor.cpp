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
	//mCollisionBox->SetCollisionProfileName(TEXT("PlayerSword"));
	mCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	mCollisionCapsule->SetupAttachment(mRoot);
	//mCollisionCapsule->SetCollisionProfileName(TEXT("PlayerSword"));
	mCollisionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	mCollisionSphere->SetupAttachment(mRoot);
	//mCollisionSphere->SetCollisionProfileName(TEXT("PlayerSword"));
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
	//PrintViewport(1.f, FColor::Black, TEXT("GetHit"));
}

void ACollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mIsMove)
	{
		/*if (mCollisionShape == ECollisionType::ECS_Box)
			mCollisionBox->SetWorldLocation(mInitLocation + mMoveDirection * DeltaTime * 1000.f);
		if (mCollisionShape == ECollisionType::ECS_Capsule)
			mCollisionCapsule->SetWorldLocation(mInitLocation + mMoveDirection * DeltaTime * 1000.f);*/
	}
}

void ACollisionActor::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(1.f, FColor::Black, TEXT("OnBoxOverlapBegin"));

	// test
	float damage = 10.f;

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
			if (Cast<AValkyrieSlash>(mParent))
				Cast<AValkyrieSlash>(mParent)->SetIsHit(true);
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
					if (Cast<AValkyrieSlash>(mParent))
						Cast<AValkyrieSlash>(mParent)->SetIsHit(true);

					}),
				0.9f,	// 몇 초 간격으로 피해를 줄 것인가. (test)
				true
				);
		}
	}
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

		if (Cast<AValkyrieSlash>(mParent))
		{
			Cast<AValkyrieSlash>(mParent)->SetIsHit(true);
		}
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
