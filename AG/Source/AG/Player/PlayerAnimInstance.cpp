// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mCurPlayMode = PLAYE_MODE::BATTLE;
	mPlayerState = PLAYER_STATE::IDLE;

	mPlayModeValue = 2.f;
	mSpeedValue = 0.f;

	mMovingTime = 0.f;
	mIsRun = false;
	
	mRunInterpTime = 0.f;
	mIsRunInterp = false;
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
	// Walk <-> Run ��ȯ.
	//------------------------
	if (IsValid(playerCharacter))
	{
		UCharacterMovementComponent* movement = playerCharacter->GetCharacterMovement();

		// Run ������ ���
		if (mIsRun)
		{
			// ó�� Run���� �ٲ���� ��� mSpeedValue�� 1 ~ 3 ���� �ٲ� ��, �����Ͽ� ���� ������.
			if (mIsRunInterp)
			{
				// 1 ~ 3���� �ٲ��� ��.
				mRunInterpTime += DeltaSeconds;
				mSpeedValue = FMath::Lerp(1.f, 3.f, mRunInterpTime);


				// mSpeedValue�� 3.0f �̻��� �Ǹ� �� �������ʹ� ������ ���� �ʾƵ� ��.
				if (mSpeedValue >= 3.0f)
				{
					mIsRunInterp = false;
					mRunInterpTime = 0.f;
				}
				//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("ing, speed: %f"), mSpeedValue));
			}
			// Run �����̰�, mSpeedValue ���� 3.0f �̻��� �Ǿ� ������ �� �ʿ䰡 ���� ���.
			else
			{
				mSpeedValue = movement->Velocity.Size() / (movement->MaxWalkSpeed / 3.f);
				//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("finish, speed: %f"), mSpeedValue));
			}
		}
		// Walk ������ ���.
		else
		{
			mSpeedValue = movement->Velocity.Size() / movement->MaxWalkSpeed;	// MaxWalkSpeed: 600
		}

		//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("mSpeedValue: %f"), mSpeedValue));


		// �������� ���߸� MOVE -> IDLE ����.
		if (mSpeedValue <= 0.f)
		{
			mIsRun = false;
			mPlayerState = PLAYER_STATE::IDLE;
		}
		else
			mPlayerState = PLAYER_STATE::MOVE;



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

void UPlayerAnimInstance::ChangePlayMode()
{
	// ���� �����̴� ���̸� ��Ÿ�ִ� ������� �ʰ�, �÷��� ��常 ����.
	if (mPlayerState != PLAYER_STATE::IDLE)
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
