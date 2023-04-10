// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"

#include "Monster.h"
#include "MonsterAIController.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	mMonsterMotionType = MONSTER_MOTION::IDLE;

	mHitAdditive = 0.f;
	mHitMontageIndex = 0;
	mHitDirection = TEXT("");

	mIsHit = false;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mMonsterMotionType == MONSTER_MOTION::ATTACK)
	{
		//if (!Montage_IsPlaying(mAttackMontage))
		//{
		//	Montage_SetPosition(mAttackMontage, 0.f);
		//	Montage_Play(mAttackMontage);
		//	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
		//	if (IsValid(monster))
		//		monster->SetIsAttackEnd(false);
		//}
	}
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	TryGetPawnOwner()->Destroy();
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
	mIsHit = false;
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	AMonsterAIController* controller = Cast<AMonsterAIController>(monster->GetController());
	controller->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHitEnd"), false);
	monster->SetIsAttackEnd(true);
	PrintViewport(10.f, FColor::Purple, TEXT("hit end"));
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	mMonsterMotionType = MONSTER_MOTION::IDLE;
	if (IsValid(monster))
		monster->SetIsAttackEnd(true);
}

void UMonsterAnimInstance::AnimNotify_AttackCheck()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
		monster->NormalAttackCheck();
}

void UMonsterAnimInstance::Hit()
{
	if (!IsValid(mHitMontageArray[mHitMontageIndex]))
		return;

	mHitAdditive = 1.f;

	if (!Montage_IsPlaying(mHitMontageArray[mHitMontageIndex]))
	{
		//PrintViewport(10.f, FColor::Purple, TEXT("hitttttttt"));
		Montage_SetPosition(mHitMontageArray[mHitMontageIndex], 0.f);
		Montage_Play(mHitMontageArray[mHitMontageIndex]);
	}
}

void UMonsterAnimInstance::SetHitDirection(FString _value)
{
	mHitDirection = _value;
	mIsHit = true;

	if (_value == TEXT("Front"))
	{
		mHitMontageIndex = 0;
	}
	else if (_value == TEXT("Back"))
	{
		mHitMontageIndex = 1;
	}
	else if (_value == TEXT("Left"))
	{
		mHitMontageIndex = 2;
	}
	else if (_value == TEXT("Right"))
	{
		mHitMontageIndex = 3;
	}

	
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	AMonsterAIController* controller = Cast<AMonsterAIController>(monster->GetController());
	controller->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHitEnd"), true);
	monster->SetIsAttackEnd(true);
	Hit();
	//PrintViewport(2.f, FColor::Blue, mHitDirection);
}

void UMonsterAnimInstance::Attack()
{
	mMonsterMotionType = MONSTER_MOTION::ATTACK;

	if (!Montage_IsPlaying(mAttackMontage))
	{
		Montage_SetPosition(mAttackMontage, 0.f);
		Montage_Play(mAttackMontage);

		AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

		if (IsValid(monster))
			monster->SetIsAttackEnd(false);

	}
}
