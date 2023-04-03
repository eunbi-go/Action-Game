// Fill out your copyright notice in the Description page of Project Settings.


#include "ContinuousSkill.h"
#include "../Particle/ParticleNiagara.h"

AContinuousSkill::AContinuousSkill()
{
	mIsNiagaraActive = false;

}

void AContinuousSkill::BeginPlay()
{
	Super::BeginPlay();
	mNiagara->SetRelativeScale3D(FVector(0.005f, 0.005f, 0.005f));
}

void AContinuousSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AContinuousSkill::CollisionProjectile(const FHitResult& Hit)
{
}
