// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieFallingSword.h"
#include "../../Player/Valkyrie.h"
#include "../../Particle/ParticleNiagara.h"

AValkyrieFallingSword::AValkyrieFallingSword()
{
}

void AValkyrieFallingSword::Activate()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetMp(-10.f);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, true);
		valkyrie->SetSkillState(ESkillState::ESS_HardAttack);
		valkyrie->PlayMontage(FName("HardAttack"));
		valkyrie->SetMontagePlayRate(FName("HardAttack"), 0.1f);
	}
}

void AValkyrieFallingSword::BeginPlay()
{
	Super::BeginPlay();
}

void AValkyrieFallingSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieFallingSword::SpawnEffect()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->CameraSwitch(true);
	}
	
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector location = mOwner->GetActorLocation();
	FVector forward = mOwner->GetActorForwardVector();

	AParticleNiagara* slash = GetWorld()->SpawnActor<AParticleNiagara>(
		location + forward * 100.f,
		FRotator::ZeroRotator,
		SpawnParam
	);
	slash->SetParticle(TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_SwordAttack.NS_SwordAttack'"));
}

void AValkyrieFallingSword::SkillEnd()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->CameraSwitch(false);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
		valkyrie->SetSkillState(ESkillState::ESS_None);
	}
	Destroy();
}

