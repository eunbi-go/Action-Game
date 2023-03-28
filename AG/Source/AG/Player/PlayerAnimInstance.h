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
enum class PLAYER_MOTION : uint8
{
	IDLE, MOVE, JUMP, PLAYER_STATE_END
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
	UFUNCTION()
	void AnimNotify_JumpEnd();

	UFUNCTION()
	void AnimNotify_EquipSword();



public:
	PLAYER_MOTION GetPlayerMotion() { return mPlayerState; }
	bool GetIsRun() { return mIsRun; }


public:
	void SetPlayModeValue(float _value) { mPlayModeValue = _value; }
	void SetSpeedValue(float _value) { mSpeedValue = _value; }
	void SetIsLandStart(bool _value) { mIsLandStart = _value; }
	void ChangePlayMode();
	void Evade(DIRECTION direction);
	void JumpStart();
	void Dash();
	void FinishDash();
	void EquipWeapon();


protected:
	PLAYE_MODE		mCurPlayMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	PLAYER_MOTION	mPlayerState;


	// Idle1 <-> Idle2 �ٲ� �� ����� ��Ÿ�� �迭.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mIdleModeMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mEvadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage*	mEquipWeaponMontage;

	// Idle_Move BlendSpace.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mPlayModeValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpeedValue;
	


	// Walk -> Run ���� �ٲ�� ���� �ð� ���.
	float	mMovingTime;
	// ���� Move(Run) �����ΰ�.
	bool	mIsRun;


	// Run ������ ���� �ð� ����.
	float	mRunInterpTime;
	// Run ������ �ϴ� ���ΰ�.
	bool	mIsRunInterp;



	// ���� Dash �����ΰ�.
	bool	mIsDash;
	bool	mIsInitDash;

	// ���߿� �ִ°�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool	mIsGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool	mIsLandStart;
};
