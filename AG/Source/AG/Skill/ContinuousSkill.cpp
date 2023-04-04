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

	mNiagara->SetRelativeScale3D(FVector(0.001f, 0.001f, 0.001f));
	mNiagara->OnSystemFinished.AddDynamic(this, &AContinuousSkill::Finish);
}

void AContinuousSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AContinuousSkill::Finish(UNiagaraComponent* comp)
{
	PrintViewport(10.f, FColor::Yellow, TEXT("AContinuousSkill::Finish"));
	mOnNiagaraEnd.Broadcast(this, comp);
}
