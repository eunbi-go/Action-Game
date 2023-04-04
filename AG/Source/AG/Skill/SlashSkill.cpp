// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSkill.h"
#include "../Particle/ParticleNiagara.h"

ASlashSkill::ASlashSkill()
{
}

void ASlashSkill::BeginPlay()
{
	Super::BeginPlay();

	mNiagara->OnSystemFinished.AddDynamic(this, &ASlashSkill::Finish);
}

void ASlashSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashSkill::Finish(UNiagaraComponent* comp)
{
	PrintViewport(10.f, FColor::Yellow, TEXT("ASlashSkill::Finish"));
	mOnNiagaraEnd.Broadcast(this, comp);
}