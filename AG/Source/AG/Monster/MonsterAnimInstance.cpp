// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"

#include "Monster.h"


UMonsterAnimInstance::UMonsterAnimInstance()
{
	mMonsterMotionType = MONSTER_MOTION::IDLE;

	mHitAdditive = 0.f;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	TryGetPawnOwner()->Destroy();
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
		monster->SetIsAttackEnd(true);
}

void UMonsterAnimInstance::Hit()
{
	if (!IsValid(mHitMontage))
		return;

	mHitAdditive = 1.f;

	if (!Montage_IsPlaying(mHitMontage))
	{
		Montage_SetPosition(mHitMontage, 0.f);
		Montage_Play(mHitMontage);
	}
}