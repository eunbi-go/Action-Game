// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSlash.h"
#include "../Interface/HitInterface.h"
#include "../Collision/CollisionActor.h"

void AValkyrieSlash::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieSlash::Stop, 0.1f, false);



}

void AValkyrieSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	location += mDirection * 1000.f * DeltaTime;
	SetActorLocation(location);
	if (IsValid(mLeftCollisionActor))
		mLeftCollisionActor->mCollisionBox->SetWorldLocation(location);
}

void AValkyrieSlash::OnParticleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(1.f, FColor::Black, TEXT("ValkyrieSlash OverlapBegin"));

	if (IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor))
	{
		hitInterface->GetHit(SweepResult.ImpactPoint);
	}
}

void AValkyrieSlash::Stop()
{
	mParticle->SetPaused(true);
	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieSlash::Start, 0.2f, false);
}

void AValkyrieSlash::Start()
{
	mParticle->ResetSystem();

	mParticle->OnComponentBeginOverlap.AddDynamic(this, &AValkyrieSlash::OnParticleOverlapBegin);

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mLeftCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParam
		);
	mLeftCollisionActor->SetBoxExtent(FVector(50.f));
	mLeftCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mLeftCollisionActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	mParticle->SetVisibility(false);
	mParticle->SetGenerateOverlapEvents(true);
	mParticle->bMultiBodyOverlap = true;
}
