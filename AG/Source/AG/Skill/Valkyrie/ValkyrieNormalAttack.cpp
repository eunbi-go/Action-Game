// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieNormalAttack.h"
#include "../../Player/Valkyrie.h"
#include "../../Particle/ParticleNiagara.h"

AValkyrieNormalAttack::AValkyrieNormalAttack()
{
}

void AValkyrieNormalAttack::Activate()
{

}

void AValkyrieNormalAttack::BeginPlay()
{
	Super::BeginPlay();
}

void AValkyrieNormalAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieNormalAttack::StartNormalAttack()
{
	if (mCurrentAttackIndex == 4)
		mCurrentAttackIndex = 1;
	mIsCanNextAttack = true;
	mIsAttackInputOn = false;
	mCurrentAttackIndex = FMath::Clamp<int32>(mCurrentAttackIndex + 1, 1, mAttackMaxIndex);
	if (AValkyrie* player = Cast<AValkyrie>(mOwner))
	{
		player->SetActionState(EActionState::EAS_Attack_Skill, true);
		player->SetSkillState(ESkillState::ESS_NormalAttack);
	}
}

void AValkyrieNormalAttack::EndNormalAttack()
{
	mIsAttackInputOn = false;
	mIsCanNextAttack = false;
	mCurrentAttackIndex = 0;
	mAttackMaxIndex = 4;
	if (AValkyrie* player = Cast<AValkyrie>(mOwner))
	{
		player->SetActionState(EActionState::EAS_Attack_Skill, false);
		player->SetSkillState(ESkillState::ESS_None);
	}
}

void AValkyrieNormalAttack::SpawnEffect()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	FVector location = mOwner->GetActorLocation() - (mOwner->GetActorForwardVector() * 100.f);
	location.Z = mOwner->GetActorLocation().Z + (Cast<ACharacter>(mOwner)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f);

	if (mCurrentAttackIndex == 1 || mCurrentAttackIndex == 2 || mCurrentAttackIndex == 3)
	{
		AParticleNiagara* effect = GetWorld()->SpawnActor<AParticleNiagara>(
			location,
			FRotator::ZeroRotator,
			SpawnParam
		);

		if (mCurrentAttackIndex == 3)
			effect->SetActorScale3D(FVector(0.3f, 0.5f, 0.5f));
		else
			effect->SetActorScale3D(FVector(0.5f));
		if (mCurrentAttackIndex == 2)
			effect->AddActorLocalRotation(FRotator(180.f, 0.f, 0.f));

		effect->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_SlashAtt_Sl_04.NS_SlashAtt_Sl_04'"));
	}
	else
	{
		AParticleNiagara* effect = GetWorld()->SpawnActor<AParticleNiagara>(
			location,
			FRotator::ZeroRotator,
			SpawnParam
		);
		effect->SetParticle(TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Blink_Fire.NS_Blink_Fire'"));
	}

}

void AValkyrieNormalAttack::SkillEnd()
{
}

void AValkyrieNormalAttack::InputPressed()
{
	if (mIsAttacking)
	{
		if (mIsCanNextAttack)
			mIsAttackInputOn = true; 
	}
	else
	{

		mIsAttacking = true;
		StartNormalAttack();
		if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
			valkyrie->PlayMontage(FName("Attack"), FName(*FString::Printf(TEXT("Attack%d"), mCurrentAttackIndex)));
	}
}

void AValkyrieNormalAttack::Notify_NextAttackCheck()
{
	mIsCanNextAttack = false;
	if (mIsAttackInputOn)
	{
		StartNormalAttack();
		if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
			valkyrie->PlayMontage(FName("Attack"), FName(*FString::Printf(TEXT("Attack%d"), mCurrentAttackIndex)));
	}
}

void AValkyrieNormalAttack::Notify_AttackEnd()
{
	CustomTimeDilation = 1.f;
	mIsAttacking = false;
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
		valkyrie->SetSkillState(ESkillState::ESS_None);
	}
	EndNormalAttack();
}

