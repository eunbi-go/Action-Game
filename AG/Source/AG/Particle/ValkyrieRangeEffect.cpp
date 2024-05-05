// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieRangeEffect.h"
#include "../Collision/CollisionActor.h"
#include "ParticleCascade.h"

AValkyrieRangeEffect::AValkyrieRangeEffect()
{
}

void AValkyrieRangeEffect::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mParticle->OnSystemFinished.AddDynamic(this, &AValkyrieRangeEffect::OnSystemFinish);

	mCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParam
	);

	mCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCollisionActor->SetParent(this);
	mCollisionActor->SetOwnerActor(this);

}

void AValkyrieRangeEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!mIsInitCollisionActor)
	{
		mIsInitCollisionActor = true;
		mCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
		mCollisionActor->SetBoxExtent(FVector(50.f));
		mCollisionActor->SetHiddenInGame(false);
		mCollisionActor->SetRelativeScale(mCollisionSize);
		mCollisionActor->SetRelativeRotation(mCollisionRotator);
	}
}

void AValkyrieRangeEffect::SpawnHitEffect(const FVector& Location, const FRotator& Rotator)
{
	//PrintViewport(3.f, FColor::Yellow, FString("AValkyrieRangeEffect::SpawnHitEffect"));
	//PrintViewport(3.f, FColor::Yellow, FString::Printf(TEXT("%f %f %f"), Location.X, Location.Y, Location.Z));
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleNiagara* Particle =
		GetWorld()->SpawnActor<AParticleNiagara>(
			Location,
			FRotator::ZeroRotator,
			SpawnParam);

	Particle->SetParticle(TEXT("NiagaraSystem'/Game/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_BasicHit.NS_BasicHit'"));
	Particle->SetActorScale3D(FVector(4.f));
}

void AValkyrieRangeEffect::OnSystemFinish(UNiagaraComponent* PSystem)
{
	if (IsValid(mCollisionActor))
		mCollisionActor->Destroy();
	Destroy();
}

void AValkyrieRangeEffect::SetCollisionSize(const FVector& Size)
{
	mCollisionSize = Size;
}

void AValkyrieRangeEffect::SetCollisionRotate(const FRotator& Rotator)
{
	mCollisionRotator = Rotator;
}


