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


	if (mHitType == EHitType::EHT_Once)
	{
		if (Cast<AValkyrieSlash>(mParent) && Cast<AValkyrieSlash>(mParent)->GetIsHit())
			return;

		// 한 번에 한 액터를 2번 이상 hit 하지 않도록.
		for (AActor* actor : mActorsToIgnoreArray)
			mActorsToIgnoreArray.AddUnique(actor);
	}

	TArray<FHitResult> boxHit;
	UKismetSystemLibrary::BoxTraceMulti(
		this,
		mCollisionBox->GetComponentLocation(),
		mCollisionBox->GetComponentLocation() + mCollisionBox->GetForwardVector() * 100.f,
		FVector(50.f),
		mCollisionBox->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		mActorsToIgnoreArray,
		EDrawDebugTrace::ForDuration,
		boxHit,
		true	// 자기 자신은 무시한다.
	);

	float damage = 10.f;

	for (auto hitInfo : boxHit)
	{
		if (hitInfo.GetActor())
		{
			IHitInterface* hitInterface = Cast<IHitInterface>(hitInfo.GetActor());

			if (mHitType == EHitType::EHT_Once)
			{
				if (hitInterface)
				{
					hitInterface->GetHit(hitInfo.ImpactPoint);
				}
				hitInfo.GetActor()->TakeDamage(
					damage,
					FDamageEvent(),
					GetWorld()->GetFirstPlayerController(),
					this);
				if (Cast<AValkyrieSlash>(mParent))
					Cast<AValkyrieSlash>(mParent)->SetIsHit(true);
				if (Cast<AValkyrieDemonSlash>(mParent))
				{
					hitInterface->PlayJumpAction(hitInfo.ImpactPoint);
				}
			}	
			else if (mHitType == EHitType::EHT_Continuous)
			{
				GetWorld()->GetTimerManager().SetTimer(
					mTimer,
					FTimerDelegate::CreateLambda([this, hitInfo, damage, hitInterface]() {
						
						PrintViewport(1.f, FColor::Black, TEXT("GetHit"));
						if (hitInterface)
						{
							hitInterface->GetHit(hitInfo.ImpactPoint);
						}
						hitInfo.GetActor()->TakeDamage(
							damage,
							FDamageEvent(),
							GetWorld()->GetFirstPlayerController(),
							this);
						if (Cast<AValkyrieSlash>(mParent))
							Cast<AValkyrieSlash>(mParent)->SetIsHit(true);

						}),
					0.7f,
					true
					);
			}
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
		if (Cast<AValkyrieDemonSlash>(mParent) && hitInterface)
		{
			hitInterface->PlayJumpAction(SweepResult.ImpactPoint);
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
