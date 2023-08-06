// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieAnimInstance.h"


void UValkyrieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mCharacter = Cast<AValkyrie>(TryGetPawnOwner());
	if (mCharacter)
	{
		mMovementComp = mCharacter->GetCharacterMovement();
	}

	mIsInAir = false;
	mIsJumpAttackEnd = true;
}

void UValkyrieAnimInstance::NativeUpdateAnimation(float _deltaTime)
{
	Super::NativeUpdateAnimation(_deltaTime);

	if (mMovementComp)
	{
		mGroundSpeed = UKismetMathLibrary::VSizeXY(mMovementComp->Velocity);
		FRotator rotation = mCharacter->GetControlRotation();
		mDirection = mCharacter->GetDirection();
		//mCharacterState = mCharacter->GetCharacterState();
		mIsInAir = mMovementComp->IsFalling();
		mIsJumpAttackEnable = mCharacter->GetIsJumpAttackEnable();
		mActionState = mCharacter->GetActionState();
		mJumpAttackIndex = mCharacter->GetJumpAttackIndex();
	}
}

void UValkyrieAnimInstance::AnimNotify_Unequip()
{
	mCharacter->UnequipSword();
}

void UValkyrieAnimInstance::AnimNotify_AttackEnable()
{
	mOnNextAttackCheck.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_AttackCheckStart()
{
	mOnAttackCheckStart.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_AttackEnd()
{
	mOnAttackEnd.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_SkillEnd()
{
	mSkillEnd.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_Launch()
{
	mOnLaunch.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_SpawnEffect()
{
	mCharacter->SpawnEffect();
}

void UValkyrieAnimInstance::AnimNotify_ChangeCamera()
{
	mChangeCamera.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_JumpAttackEnable()
{
	mJumpAttackEnable.Broadcast();
	mIsJumpAttackEnd = true;
}

void UValkyrieAnimInstance::AnimNotify_JumpEnd()
{
	mOnJumpEnd.Broadcast();
	mIsJumpAttackEnd = true;
}

void UValkyrieAnimInstance::AnimNotify_JumpAttackEnd()
{
	mIsJumpAttackEnd = false;
}

void UValkyrieAnimInstance::AnimNotify_JumpAttackStart()
{
	//mIsJumpAttackEnd = false;
}
