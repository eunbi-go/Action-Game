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
	IDLE, MOVE, JUMP, NORMAL_ATTACK, SKILL, PLAYER_STATE_END
};

UENUM(BlueprintType)
enum class DIRECTION : uint8
{
	FORWARD, BACKWARD, DIRECTION_END
};

USTRUCT(BlueprintType)
struct FSkillAnimationInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		SKILL_TYPE	skillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UAnimMontage* Montage;
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

	UFUNCTION()
	void AnimNotify_AttackCheck();

	UFUNCTION()
	void AnimNotify_AttackEnable();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
		void AnimNotify_Delay();

	UFUNCTION()
		void AnimNotify_Reset();

	UFUNCTION()
		void AnimNotify_SkillEnd();

	UFUNCTION()
		void AnimNotify_StartGauge();

	UFUNCTION()
		void AnimNotify_LastNormalAttack();

	UFUNCTION()
		void AnimNotify_Teleport();

	UFUNCTION()
		void AnimNotify_TeleportEff();

	UFUNCTION()
		void AnimNotify_ArriveSprintPoint();

	UFUNCTION()
		void AnimNotify_ResetSpeed();

	UFUNCTION()
		void AnimNotify_Continuous();

	UFUNCTION()
		void AnimNotify_EffectSpawn();

	UFUNCTION()
		void AnimNotify_ContinuousEff();

	UFUNCTION()
		void AnimNotify_SlowStop();

	UFUNCTION()
		void AnimNotify_SlashEff();

	UFUNCTION()
		void AnimNotify_SlashCS();

	UFUNCTION()
		void AnimNotify_ConEff();

	UFUNCTION()
		void AnimNotify_TarilOn();

	UFUNCTION()
		void AnimNotify_TarilOff();


public:
	PLAYER_MOTION GetPlayerMotion() { return mPlayerState; }
	bool GetIsRun() { return mIsRun; }
	SKILL_TYPE GetCurSkillType() { return mCurSkillType; }
	int GetNormalAttackIndex() { return mNormalAttackIndex; }

public:
	void SetPlayModeValue(float _value) { mPlayModeValue = _value; }
	void SetSpeedValue(float _value) { mSpeedValue = _value; }
	void SetIsLandStart(bool _value) { mIsLandStart = _value; }
	void SetIsPlayerMotion(PLAYER_MOTION _value) { mPlayerState = _value; }


public:
	void ChangePlayMode();
	void Evade(DIRECTION _direction);
	void JumpStart();
	void Dash();
	void FinishDash();
	void EquipWeapon();
	void NormalAttack();
	void UseSkill(SKILL_TYPE _skillType);
	void GaugeEnd();
	void RestartSkill();
	void ResumSprintAttackMontage();
	void ReplaySprintMontage();
	void StopContinuousSkill();

protected:
	PLAYE_MODE		mCurPlayMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	PLAYER_MOTION	mPlayerState;


	// Idle1 <-> Idle2 바뀔 때 재생할 몽타주 배열.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mIdleModeMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mEvadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage*	mEquipWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> mNormalAttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FSkillAnimationInfo>	mSkillMontageArray;

	UPROPERTY()
	FTimerHandle timerHandle;

protected:
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



	// 현재 Dash 상태인가.
	bool	mIsDash;
	bool	mIsInitDash;

	// 공중에 있는가.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsLandStart;




	//--------------------------
	// Normal Attack.
	//--------------------------
	// 공격 가능 여부.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsNormalAttackEnable;

	// 현재 공격 상태 확인.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mIsNormalAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mNormalAttackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mNormalAttackShake2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mTel;

	//--------------------------
	// Skill
	//--------------------------
	SKILL_TYPE	mCurSkillType;
	int mCurSkillPlayingIndex;

	int mSprintCount;

	int mContinuousCount;
	bool mIsEndContinuous;
};
