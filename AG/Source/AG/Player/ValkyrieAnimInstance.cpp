// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieAnimInstance.h"
#include "../Particle/ParticleNiagara.h"
#include "Valkyrie.h"
#include "AGPlayer.h"

UValkyrieAnimInstance::UValkyrieAnimInstance()
{
}

void UValkyrieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mCharacter = Cast<AValkyrie>(TryGetPawnOwner());
	if (IsValid(mCharacter))
	{
		mMovementComp = mCharacter->GetCharacterMovement();
		PrintViewport(3.f, FColor::Yellow, FString("Character"));
	}
	else
		PrintViewport(3.f, FColor::Yellow, FString("No Character"));

	mIsInAir = false;
	mIsJumpAttackEnd = true;

}

void UValkyrieAnimInstance::NativeUpdateAnimation(float _deltaTime)
{
	Super::NativeUpdateAnimation(_deltaTime);

	if (IsValid(mMovementComp))
	{
		mGroundSpeed = UKismetMathLibrary::VSizeXY(mMovementComp->Velocity);
		FRotator rotation = mCharacter->GetControlRotation();
		mDirection = mCharacter->GetDirection();
		//mCharacterState = mCharacter->GetCharacterState();
		mIsInAir = mMovementComp->IsFalling();
		mIsCrouch = mCharacter->CheckActionState(EActionState::EAS_Crouch, false);
		mIsGuard = mCharacter->CheckActionState(EActionState::EAS_Guard, false);


		// ���Ͻ� ��Ī�� �ʿ��� ���� ����.
		// ���� ���ӵ� ��ȯ
		mAccelation = mMovementComp->GetCurrentAcceleration();
		mAccelation.Z = 0.f;
		if (!mAccelation.IsNearlyZero())
		{
			mIsAccel = true;
		}
		else
			mIsAccel = false;

		// ���� �ӵ� ��ȯ
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


		// �÷��̾��� ����� ���� �ӵ��� ���̸� ��Ÿ���� rotator
		
		mLocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(mVelocity, mCharacter->GetRootComponent()->GetComponentRotation());
		//PrintViewport(0.5f, FColor::Blue, FString::Printf(TEXT("distance: %f"), mLocalVelocityDirectionAngle));
		// ������ ������ ���� ���� ��/��/�ڷ� ���� �ִ��� Ȯ���Ѵ�.
		if (abs(mLocalVelocityDirectionAngle) <= 65.f)
		{
			// ����
			
			//PrintViewport(0.5f, FColor::Yellow, FString("Add EMD_Front"));
			mMoveDirection = EMoveDirection::EMD_Front;
		}
		else if (abs(mLocalVelocityDirectionAngle) >= 110.f)
		{
			// ��
			//PrintViewport(0.5f, FColor::Yellow, FString("Add EMD_Back"));
			mMoveDirection = EMoveDirection::EMD_Back;
		}
		else if (mLocalVelocityDirectionAngle >= 0.f)
		{
			// ����
			//PrintViewport(0.5f, FColor::Yellow, FString("Add EMD_Left"));
			mMoveDirection = EMoveDirection::EMD_Left;
		}
		else
		{
			// ������
			//PrintViewport(0.5f, FColor::Yellow, FString("Add EMD_Right"));
			mMoveDirection = EMoveDirection::EMD_Right;
		}
	}
		//PrintViewport(3.f, FColor::Yellow, FString("No Movement"));
	if (mIsCrouch && mGroundSpeed)
	{
		mCrouchValue = 1.f;
	}
	else
	{
		mCrouchValue = 0.f;
	}

	
	if (mIsGuard && mGroundSpeed)
	{
		mGuardValue = 1.f;
	}
	else
	{
		mGuardValue = 0.f;
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
	//mCharacter->SpawnEffect();
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

void UValkyrieAnimInstance::AnimNotify_HitEnd()
{
	mOnHitEnd.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_SlashEnable()
{
	mOnSlashEnable.Broadcast();
}

void UValkyrieAnimInstance::AnimNotify_Pause()
{
	//Montage_Pause();
	mOnPause.Broadcast();
}
