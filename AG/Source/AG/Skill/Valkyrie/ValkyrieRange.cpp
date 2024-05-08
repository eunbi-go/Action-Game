// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieRange.h"
#include "../../Player/Valkyrie.h"
#include "../../Collision/CollisionActor.h"
#include "../../Particle/ValkyrieRangeEffect.h"

AValkyrieRange::AValkyrieRange()
{
}

void AValkyrieRange::Activate()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetMp(-10.f);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, true);
		valkyrie->SetSkillState(ESkillState::ESS_Range);
	}
}

void AValkyrieRange::BeginPlay()
{
	Super::BeginPlay();
}

void AValkyrieRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (mInit)
	{
		mInit = false;

		// owner mp 감소
		Cast<AValkyrie>(mOwner)->SetMp(-10.f);

		// owner 스킬 상태 변경
		Cast<AValkyrie>(mOwner)->SetSkillState(ESkillState::ESS_Range);

		// owner 몽타주 느리게 재생
		Cast<AValkyrie>(mOwner)->PlayMontage(FName("Ribbon"));
		Cast<AValkyrie>(mOwner)->SetMontagePlayRate(FName("Ribbon"), 0.7f);

	}

	if (mIsPress)
	{
		//PrintViewport(1.f, FColor::Red, FString("PRessss"));
		mPressingTime += DeltaTime;
	}
}

void AValkyrieRange::SpawnEffect()
{
	FActorSpawnParameters	spawnParam;
	spawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AValkyrieRangeEffect* niagara = GetWorld()->SpawnActor<AValkyrieRangeEffect>(
		mOwner->GetActorLocation(),
		GetActorRotation(),
		spawnParam
	);
	float size = 0.6f + mPressingTime;
	if (mPressingTime == 0.f)
		size = 0.3f;
	niagara->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Cut_Sl_04.NS_Cut_Sl_04'"));
	niagara->SetActorScale3D(FVector(size));

	FVector collisionSize = FVector(2.f, 2.f, 1.f);
	size *= 2.f;
	collisionSize.X += size;
	collisionSize.Y += size;
	collisionSize.Z += size;

	niagara->SetCollisionSize(collisionSize);
	niagara->SetCollisionRotate(mOwner->GetActorForwardVector().Rotation());

}

void AValkyrieRange::SkillEnd()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
		valkyrie->SetSkillState(ESkillState::ESS_None);
	}
	Destroy();
}


