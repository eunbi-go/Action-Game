// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieAnimInstance.h"
#include "../Particle/ParticleNiagara.h"

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
		mActionState = mCharacter->GetActionState();
		mIsCrouch = mCharacter->GetIsCrouch();

		// ���Ͻ� ��Ī�� �ʿ��� ���� ����.
		mAccelation = mMovementComp->GetCurrentAcceleration();
		mAccelation.Z = 0.f;
		if (!mAccelation.IsNearlyZero())
		{
			mIsAccel = true;
		}
		else
			mIsAccel = false;

		mVelocity = mCharacter->GetVelocity();
		mVelocity.Z = 0.f;
		if (!mVelocity.IsNearlyZero())
		{
			mIsVelocity = true;
		}
		else
			mIsVelocity = false;

		// ���� �����Ӱ� ��ġ�� ���ؼ� �Ÿ��� �ӵ��� ���Ѵ�.
		FVector location = mCharacter->GetActorLocation() - mCurrentWorldLocation;
		mDistanceLastUpdate = UKismetMathLibrary::VSizeXY(location);
		mCurrentWorldLocation = location;

		// ���� �ӵ��� ���Ѵ�.
		mCurrentSpeed = UKismetMathLibrary::SafeDivide(mDistanceLastUpdate, _deltaTime);
	}
	if (mIsCrouch && mGroundSpeed)
	{
		mCrouchValue = 1.f;
	}
	else
	{
		mCrouchValue = 0.f;
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

void UValkyrieAnimInstance::AnimNotify_JumpEnd()
{
	mOnJumpEnd.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_SpawnFresnel()
{
	mSpawnFresnel.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_ResetFresnel()
{
	mResetFresnel.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_Delay()
{
	mOnDelay.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_Dash()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector location = mCharacter->GetActorLocation() - mCharacter->GetActorForwardVector() * 290.f;
	location.Z -= (mCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.f);
	FRotator rotation = mCharacter->GetActorRotation();

	AParticleNiagara* niagara = GetWorld()->SpawnActor<AParticleNiagara>(
		location,
		rotation,
		SpawnParam
		);
	niagara->SetParticle(TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Fire.NS_Dash_Fire'"));
}
