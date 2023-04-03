// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSkill.h"
#include "../Particle/ParticleNiagara.h"

ASlashSkill::ASlashSkill()
{
	mLeftNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara1"));
	//AParticleNiagara* niagara2 = CreateDefaultSubobject<AParticleNiagara>(TEXT("Niagara2"));
	mLeftNiagara->SetupAttachment(mRoot);

	
}

void ASlashSkill::BeginPlay()
{
	//mNiagara->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	UNiagaraSystem* particle = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_LavaRocksAttack_Single.NS_LavaRocksAttack_Single'"));
	mLeftNiagara->SetAsset(particle);
	mLeftNiagara->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	mLeftNiagara->SetActive(false);
}

void ASlashSkill::Tick(float DeltaTime)
{
}

void ASlashSkill::CollisionProjectile(const FHitResult& Hit)
{
}

void ASlashSkill::SpawnNiagara()
{
}
