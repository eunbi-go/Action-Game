// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"



UENUM(BlueprintType)
enum class PLAYE_MODE : uint8
{
	NO_BATTLE, BATTLE, PLAYER_MODE_END
};

UENUM(BlueprintType)
enum class PLAYER_STATE : uint8
{
	IDLE, MOVE, PLAYER_STATE_END
};

UENUM(BlueprintType)
enum class DIRECTION : uint8
{
	FORWARD, BACKWARD, DIRECTION_END
};

UCLASS()
class AG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	void SetPlayModeValue(float _value) { mPlayModeValue = _value; }
	void SetSpeedValue(float _value) { mSpeedValue = _value; }
	void ChangePlayMode();
	void Evade(DIRECTION direction);
	


protected:
	PLAYE_MODE		mCurPlayMode;
	PLAYER_STATE	mPlayerState;


	// Idle1 <-> Idle2 바뀔 때 재생할 몽타주 배열.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mIdleModeMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mEvadeMontage;

	// Idle_Move BlendSpace.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mPlayModeValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpeedValue;
	


	// Walk -> Run 으로 바뀌기 위한 시간 계산.
	float	mMovingTime;
	// 현재 Move(Run) 상태인가.
	bool	mIsRun;

	// Run 보간을 위한 시간 저장.
	float	mRunInterpTime;
	// Run 보간을 하는 중인가.
	bool	mIsRunInterp;
};
