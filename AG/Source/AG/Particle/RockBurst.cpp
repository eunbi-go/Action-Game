// Fill out your copyright notice in the Description page of Project Settings.


#include "RockBurst.h"

ARockBurst::ARockBurst()
{
	mParticle->SetCollisionProfileName(TEXT("MonsterAttack"));
	mParticle->SetNotifyRigidBodyCollision(true);
	mParticle->SetGenerateOverlapEvents(true);
}

void ARockBurst::BeginPlay()
{
	Super::BeginPlay();
	mParticle->OnParticleCollide.AddDynamic(this, &ARockBurst::ParticleColiision);
}

void ARockBurst::ParticleColiision(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat)
{
	PrintViewport(3.f, FColor::Red, TEXT("ARockBurst::ParticleColiisionss"));
}
