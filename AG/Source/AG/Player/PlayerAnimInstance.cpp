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
	// Walk <-> Run 전환.
	//------------------------
	if (IsValid(playerCharacter))
	{
		UCharacterMovementComponent* movement = playerCharacter->GetCharacterMovement();

		// Run 상태일 경우
		if (mIsRun)
		{
			// 처음 Run으로 바뀌었을 경우 mSpeedValue가 1 ~ 3 으로 바뀔 때, 보간하여 값을 지정함.
			if (mIsRunInterp)
			{
				// 1 ~ 3으로 바뀌어야 함.
				mRunInterpTime += DeltaSeconds;
				mSpeedValue = FMath::Lerp(1.f, 3.f, mRunInterpTime);


				// mSpeedValue가 3.0f 이상이 되면 그 다음부터는 보간을 하지 않아도 됨.
				if (mSpeedValue >= 3.0f)
				{
					mIsRunInterp = false;
					mRunInterpTime = 0.f;
				}
				//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("ing, speed: %f"), mSpeedValue));
			}
			// Run 상태이고, mSpeedValue 값이 3.0f 이상이 되어 보간을 할 필요가 없을 경우.
			else
			{
				mSpeedValue = movement->Velocity.Size() / (movement->MaxWalkSpeed / 3.f);
				//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("finish, speed: %f"), mSpeedValue));
			}
		}
		// Walk 상태일 경우.
		else
		{
			mSpeedValue = movement->Velocity.Size() / movement->MaxWalkSpeed;	// MaxWalkSpeed: 600
		}

		//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("mSpeedValue: %f"), mSpeedValue));


		// 움직임을 멈추면 MOVE -> IDLE 변경.
		if (mSpeedValue <= 0.f)
		{
			mIsRun = false;
			mPlayerState = PLAYER_STATE::IDLE;
		}
		else
			mPlayerState = PLAYER_STATE::MOVE;



		// walk 상태에서 1초 지나면 run 으로 바뀐다.
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
	// Idle 상태에서 동작을 바꿨을 때 몽타주 재생이 끝나면, 
	// 블렌드스페이스 mPlayModeValue 값 세팅해서 이후에도 동일한 상태 유지하도록 함.
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
	// 현재 움직이는 중이면 몽타주는 재생하지 않고, 플레이 모드만 변경.
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
