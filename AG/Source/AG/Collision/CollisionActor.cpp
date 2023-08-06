// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionActor.h"
#include "../Interface/HitInterface.h"
#include "../Particle/ValkyrieLightning.h"

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
}

void ACollisionActor::BeginPlay()
{
	Super::BeginPlay();

	mCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnBoxOverlapBegin);
	mCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnBoxOverlapEnd);

	mCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACollisionActor::OnCapsuleOverlapBegin);
	mCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &ACollisionActor::OnCapsuleOverlapEnd);

	mActorsToIgnoreArray.Add(this);
}

void ACollisionActor::GetHit(AActor* _hitActor)
{
	//PrintViewport(1.f, FColor::Black, TEXT("GetHit"));
}

void ACollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollisionActor::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(1.f, FColor::Black, TEXT("OnBoxOverlapBegin"));


	if (mHitType == EHitType::EHT_Once)
	{
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
			//IHitInterface* hitInterface = Cast<IHitInterface>(hitInfo.GetActor());
			//hitInterface->GetHit(hitInfo.ImpactPoint);

			if (mHitType == EHitType::EHT_Once)
			{
				//actorsToIgnoreArray.AddUnique(hitInfo.GetActor());
			}
			else if (mHitType == EHitType::EHT_Continuous)
			{
				GetWorld()->GetTimerManager().SetTimer(
					mTimer,
					FTimerDelegate::CreateLambda([this, hitInfo, damage]() {
						PrintViewport(1.f, FColor::Black, TEXT("GetHit"));
						//UGameplayStatics::ApplyDamage(
						//	hitInfo.GetActor(),
						//	damage,
						//	GetInstigator()->GetController(),
						//	this,
						//	UDamageType::StaticClass()
						//);
						}),
					0.5f,
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
