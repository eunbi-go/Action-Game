// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportSkill.h"
ATeleportSkill::ATeleportSkill()
{
}

void ATeleportSkill::BeginPlay()
{
	Super::BeginPlay();

	//mParticle->OnParticleDeath.AddDynamic(this, &ATeleportSkill::Finish);
	mParticle->OnSystemFinished.AddDynamic(this, &ATeleportSkill::Finish);
	mNiagara->SetActive(true);
}

void ATeleportSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleportSkill::Finish(UParticleSystemComponent* comp)
{
	PrintViewport(10.f, FColor::Yellow, TEXT("ATeleportSkill::Finish"));
	mOnPaticleEnd.Broadcast(this, comp);
}
