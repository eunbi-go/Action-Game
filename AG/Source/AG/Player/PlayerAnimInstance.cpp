// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mPlayModeValue = 0.f;
	mCurPlayMode = PLAYE_MODE::NO_BATTLE;
	mPlayerState = PLAYER_STATE::IDLE;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

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
