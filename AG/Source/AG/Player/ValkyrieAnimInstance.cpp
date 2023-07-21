// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieAnimInstance.h"
#include "Valkyrie.h"

void UValkyrieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mCharacter = Cast<AValkyrie>(TryGetPawnOwner());
	if (mCharacter)
	{
		mMovementComp = mCharacter->GetCharacterMovement();
	}

	mIsInAir = false;
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