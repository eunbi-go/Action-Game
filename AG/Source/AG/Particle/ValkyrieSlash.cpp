// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSlash.h"
#include "../Interface/HitInterface.h"
#include "../Collision/CollisionActor.h"

AValkyrieSlash::AValkyrieSlash()
{

}

void AValkyrieSlash::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieSlash::Stop, 0.1f, false);

	//
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator rot = GetActorRotation();
	mCenterCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		GetActorLocation(),
		rot,
		SpawnParam
		);
	mCenterCollisionActor->SetBoxExtent(FVector(90.f, 20.f, 20.f));
	mCenterCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCenterCollisionActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mCenterCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
	mCenterCollisionActor->SetInitLocation(GetActorLocation());
	mCenterCollisionActor->SetParent(this);

	//
	FRotator rotation = GetActorRotation();
	rotation.Yaw += 50.f;

	FVector location = GetActorLocation();
	location -= GetActorRightVector() * 100.f;
	location -= GetActorForwardVector() * 150.f;

	mRightCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		location,
		rotation,
		SpawnParam
		);
	mRightCollisionActor->SetBoxExtent(FVector(90.f, 20.f, 20.f));
	mRightCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mRightCollisionActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mRightCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
	mRightCollisionActor->SetInitLocation(location);
	mRightCollisionActor->SetParent(this);
	
	//
	rotation = GetActorRotation();
	rotation.Yaw -= 50.f;

	location = GetActorLocation();
	location += GetActorForwardVector() * 150.f;
	location -= GetActorRightVector() * 100.f;

	mLeftCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		location,
		rotation,
		SpawnParam
		);
	mLeftCollisionActor->SetBoxExtent(FVector(90.f, 20.f, 20.f));
	mLeftCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mLeftCollisionActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mLeftCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
	mLeftCollisionActor->SetInitLocation(location);
	mLeftCollisionActor->SetParent(this);
}

void AValkyrieSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	location += mDirection * 1000.f * DeltaTime;

	SetActorLocation(location);

	FVector loc = mLeftCollisionActor->GetInitLocation();
	loc += mDirection * 1000.f * DeltaTime;
	mLeftCollisionActor->SetInitLocation(loc);

	FVector loc2 = mRightCollisionActor->GetInitLocation();
	loc2 += mDirection * 1000.f * DeltaTime;
	mRightCollisionActor->SetInitLocation(loc2);

	FVector loc3 = mCenterCollisionActor->GetInitLocation();
	loc3 += mDirection * 1000.f * DeltaTime;
	mCenterCollisionActor->SetInitLocation(loc3);

	if (IsValid(mLeftCollisionActor))
		mLeftCollisionActor->mCollisionBox->SetWorldLocation(loc);
	if (IsValid(mRightCollisionActor))
		mRightCollisionActor->mCollisionBox->SetWorldLocation(loc2);
	if (IsValid(mCenterCollisionActor))
		mCenterCollisionActor->mCollisionBox->SetWorldLocation(loc3);
}

void AValkyrieSlash::OnParticleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(1.f, FColor::Black, TEXT("ValkyrieSlash OverlapBegin"));

	if (IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor))
	{
		hitInterface->GetHit(SweepResult.ImpactPoint);
	}
}

void AValkyrieSlash::OnSystemFinish(UNiagaraComponent* PSystem)
{
	mLeftCollisionActor->Destroy();
	mCenterCollisionActor->Destroy();
	mRightCollisionActor->Destroy();
	Destroy();
}

void AValkyrieSlash::SetDirection(const FVector& _value)
{
	mDirection = _value;
}

void AValkyrieSlash::Stop()
{
	mParticle->SetPaused(true);
	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieSlash::Start, 0.2f, false);
}

void AValkyrieSlash::Start()
{
	mParticle->ResetSystem();
}
