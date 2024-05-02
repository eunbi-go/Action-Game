// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieRange.h"
#include "../../Player/Valkyrie.h"
#include "../../Particle/ValkyrieSlash.h"

AValkyrieRange::AValkyrieRange()
{
}

void AValkyrieRange::Activate()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetMp(-10.f);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, true);
		valkyrie->SetSkillState(ESkillState::ESS_Ribbon);
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
		Cast<AValkyrie>(mOwner)->SetSkillState(ESkillState::ESS_Ribbon);

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
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
		mOwner->GetActorLocation(),
		GetActorRotation(),
		SpawnParam
	);
	float size = 0.6f + mPressingTime;
	if (mPressingTime == 0.f)
		size = 0.3f;
	slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Cut_Sl_04.NS_Cut_Sl_04'"));
	slash->SetActorScale3D(FVector(size));
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


