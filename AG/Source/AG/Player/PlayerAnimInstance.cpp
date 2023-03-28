// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mCurPlayMode = PLAYE_MODE::BATTLE;
	mPlayerState = PLAYER_MOTION::IDLE;

	mPlayModeValue = 2.f;
	mSpeedValue = 0.f;

	mMovingTime = 0.f;
	mIsRun = false;

	mRunInterpTime = 0.f;
	mIsRunInterp = false;

	mIsDash = false;
	mIsInitDash = false;

	mIsAir = false;
	mIsGround = true;
	mIsLandStart = false;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	//------------------------
	// Walk <-> Run <-> Dash ��ȯ.
	//------------------------
	if (IsValid(playerCharacter))
	{
		UCharacterMovementComponent* movement = playerCharacter->GetCharacterMovement();

		mIsGround = movement->IsMovingOnGround();
		mIsAir = movement->IsFalling();
		if (mIsAir)
		{
			//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("mIsAir")));
			return;
		}


		// 1. Dash ������ ���.
		if (mIsDash)
		{
			// ���� ���� �ٷ� �� ����.
			mSpeedValue = 5.f;

			if (!mIsInitDash)
			{
				mIsInitDash = true;
				playerCharacter->SetDashStsteSpeed();
			}
		}


		// 2. Run ������ ���.
		else if (mIsRun)
		{
			// ó�� Run���� �ٲ���� ��� mSpeedValue�� 1 ~ 3 ���� �ٲ� ��, �����Ͽ� ���� ������.
			if (mIsRunInterp)
			{
				playerCharacter->SetRunStateSpeed();

				// 1 ~ 3���� �ٲ��� ��.
				mRunInterpTime += DeltaSeconds;
				mSpeedValue = FMath::Lerp(1.f, 3.f, mRunInterpTime);


				// mSpeedValue�� 3.0f �̻��� �Ǹ� �� �������ʹ� ������ ���� �ʾƵ� ��.
				if (mSpeedValue >= 3.0f)
				{
					mIsRunInterp = false;
					mRunInterpTime = 0.f;
				}
			}

			// Run �����̰�, mSpeedValue ���� 3.0f �̻��� �Ǿ� ������ �� �ʿ䰡 ���� ���.
			else
			{
				mSpeedValue = movement->Velocity.Size() / (movement->MaxWalkSpeed / 3.f);
			}
		}


		// 3. Walk ������ ���.
		else
		{
			playerCharacter->SetWalkStsteSpeed();
			mSpeedValue = movement->Velocity.Size() / movement->MaxWalkSpeed;	// MaxWalkSpeed: 600
		}

		//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("mSpeedValue: %f, walkSpeed: %f"), mSpeedValue, movement->MaxWalkSpeed));





		// �������� ���߸� MOVE -> IDLE ����.
		if (mSpeedValue <= 0.f)
		{
			mIsRun = false;
			mIsRunInterp = false;
			mMovingTime = 0.f;
			mRunInterpTime = 0.f;
			mPlayerState = PLAYER_MOTION::IDLE;
		}
		else
			mPlayerState = PLAYER_MOTION::MOVE;



		// walk ���¿��� 1�� ������ run ���� �ٲ��.
		if (mSpeedValue >= 0.9f && !mIsRun)
		{
			mMovingTime += DeltaSeconds;

			if (mMovingTime >= 1.f)
			{
				PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("change")));
				mIsRun = true;
				mIsRunInterp = true;
				mMovingTime = 0.f;
			}
		}
	}



	//------------------------
	// Idle ���¿��� ������ �ٲ��� �� ��Ÿ�� ����� ������, 
	// ���彺���̽� mPlayModeValue �� �����ؼ� ���Ŀ��� ������ ���� �����ϵ��� ��.
	//------------------------
	switch (mCurPlayMode)
	{
	case PLAYE_MODE::NO_BATTLE:
		if (!Montage_IsPlaying(mIdleModeMontageArray[1]))
		{
			mPlayModeValue = 0.f;
		}
		break;

	case PLAYE_MODE::BATTLE:
		if (!Montage_IsPlaying(mIdleModeMontageArray[0]))
		{
			mPlayModeValue = 1.f;
		}
		break;
	}

}

void UPlayerAnimInstance::AnimNotify_JumpEnd()
{
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("AnimNotify_JumpEnd")));
	mPlayerState = PLAYER_MOTION::IDLE;
}

void UPlayerAnimInstance::AnimNotify_EquipSword()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->EquipWeaponToHand();
	}
}

void UPlayerAnimInstance::ChangePlayMode()
{
	// ���� �����̴� ���̸� ��Ÿ�ִ� ������� �ʰ�, �÷��� ��常 ����.
	if (mPlayerState != PLAYER_MOTION::IDLE)
	{
		switch (mCurPlayMode)
		{
		case PLAYE_MODE::NO_BATTLE:
			mCurPlayMode = PLAYE_MODE::BATTLE;
			mPlayModeValue = 1.f;
			break;

		case PLAYE_MODE::BATTLE:
			mCurPlayMode = PLAYE_MODE::NO_BATTLE;
			mPlayModeValue = 0.f;
			break;

		}
		return;
	}


	switch (mCurPlayMode)
	{
	case PLAYE_MODE::NO_BATTLE:	

		mCurPlayMode = PLAYE_MODE::BATTLE;

		if (!Montage_IsPlaying(mIdleModeMontageArray[0]))
		{
			Montage_SetPosition(mIdleModeMontageArray[0], 0.f);
			Montage_Play(mIdleModeMontageArray[0]);
		}
		break;


	case PLAYE_MODE::BATTLE:	

		mCurPlayMode = PLAYE_MODE::NO_BATTLE;

		if (!Montage_IsPlaying(mIdleModeMontageArray[1]))
		{
			Montage_SetPosition(mIdleModeMontageArray[1], 0.f);
			Montage_Play(mIdleModeMontageArray[1]);
		}
		break;
	}


}

void UPlayerAnimInstance::Evade(DIRECTION direction)
{
	int index = (uint8)direction;

	if (!Montage_IsPlaying(mEvadeMontage[index]))
	{
		Montage_SetPosition(mEvadeMontage[index], 0.f);
		Montage_Play(mEvadeMontage[index]);
	}
}

void UPlayerAnimInstance::JumpStart()
{
	PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("JUMP")));
	mPlayerState = PLAYER_MOTION::JUMP;
	mIsLandStart = false;
}

void UPlayerAnimInstance::Dash()
{
	mIsDash = true;
	mIsInitDash = false;
}

void UPlayerAnimInstance::FinishDash()
{
	mIsDash = false;
	mIsInitDash = false;
}

void UPlayerAnimInstance::EquipWeapon()
{
	if (!Montage_IsPlaying(mEquipWeaponMontage))
	{
		Montage_SetPosition(mEquipWeaponMontage, 0.f);
		Montage_Play(mEquipWeaponMontage);
	}
}
