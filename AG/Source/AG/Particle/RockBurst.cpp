// Fill out your copyright notice in the Description page of Project Settings.


#include "RockBurst.h"

ARockBurst::ARockBurst()
{
	mParticle->SetCollisionProfileName(TEXT("MonsterAttack"));
	mParticle->SetNotifyRigidBodyCollision(true);
	mParticle->SetGenerateOverlapEvents(true);

	mIsCollide = false;
}

void ARockBurst::BeginPlay()
{
	Super::BeginPlay();

	mParticle->OnParticleCollide.AddDynamic(this, &ARockBurst::ParticleColiision);
	mParticle->OnComponentHit.AddDynamic(this, &ARockBurst::ParticleHit);
	GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &ARockBurst::Spawn, 0.2f, false);
}

void ARockBurst::ParticleColiision(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat)
{
	if (!mIsCollide)
	{
		mIsCollide = true;
		mOnHit.Broadcast();
		PrintViewport(3.f, FColor::Red, TEXT("ARockBurst::ParticleColiisionss"));
	}
}

void ARockBurst::ParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//mOnHit.Broadcast(OtherActor);
}

void ARockBurst::Spawn()
{
	mReSpawn.Broadcast(this);
}
