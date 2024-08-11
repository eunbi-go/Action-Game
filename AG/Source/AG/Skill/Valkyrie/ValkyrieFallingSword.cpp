// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieFallingSword.h"
#include "../../Player/Valkyrie.h"
#include "../../Particle/ParticleNiagara.h"
#include "../../Collision/CollisionActor.h"

AValkyrieFallingSword::AValkyrieFallingSword()
{
}

void AValkyrieFallingSword::Activate()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetMp(-10.f);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, true);
		//valkyrie->SetSkillState(ESkillState::ESS_HardAttack);
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
	
	FActorSpawnParameters	spawnParam;
	spawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector location = mOwner->GetActorLocation();
	FVector forward = mOwner->GetActorForwardVector();
	location += forward * 100.f;
	location.Z = 0.f;

	AParticleNiagara* niagara = GetWorld()->SpawnActor<AParticleNiagara>(
		location,
		FRotator::ZeroRotator,
		spawnParam
	);
	niagara->SetParticle(TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_SwordAttack.NS_SwordAttack'"));
	niagara->GetNiagara()->OnSystemFinished.AddDynamic(this, &AValkyrieFallingSword::ParticleFinish);

	mCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		location,
		FRotator::ZeroRotator,
		spawnParam
	);
	mCollisionActor->SetOwnerActor(niagara);
	mCollisionActor->SetHitType(EHitType::EHT_Continuous);
	mCollisionActor->SetHitSeconds(0.5f);
	mCollisionActor->SetIsContinuousHit(true);
	mCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCollisionActor->SetParent(this);
	mCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
	mCollisionActor->SetBoxExtent(FVector(50.f));
	mCollisionActor->SetRelativeScale(FVector(1.f));
	//mCollisionActor->SetHiddenInGame(false);
}

void AValkyrieFallingSword::SkillEnd()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->CameraSwitch(false);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
		valkyrie->SetSkillState(ESkillState::ESS_None);
	}
	//mCollisionActor->ClearTimer();
	mCollisionActor->Destroy();
	Destroy();
}

void AValkyrieFallingSword::ParticleFinish(UNiagaraComponent* PSystem)
{
	if (IsValid(mCollisionActor))
	{
		PrintViewport(2.f, FColor::Blue, FString("Falling Sword Finish"));
		mCollisionActor->SetIsContinuousHit(false);
	}
		
}

