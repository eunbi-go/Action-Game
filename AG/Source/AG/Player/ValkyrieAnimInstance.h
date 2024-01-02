// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Animation/AnimInstance.h"
#include "ValkyrieAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackCheckStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLaunchDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSkillEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnChangeCameraDelegate);
DECLARE_MULTICAST_DELEGATE(FOnJumpEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSpawnFresnelDelegate);
DECLARE_MULTICAST_DELEGATE(FOnResetFresnelDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDelayDelegate);

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	EMD_Front UMETA(DisplayName = "Front"),
	EMD_Back UMETA(DisplayName = "Back"),
	EMD_Right UMETA(DisplayName = "Right"),
	EMD_Left UMETA(DisplayName = "Left"),
};

UCLASS()
class AG_API UValkyrieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UValkyrieAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _deltaTime) override;

	UFUNCTION()
	void AnimNotify_Unequip();

	UFUNCTION()
	void AnimNotify_AttackEnable();

	UFUNCTION()
	void AnimNotify_AttackCheckStart();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_SkillEnd();

	UFUNCTION()
	void AnimNotify_Launch();

	UFUNCTION()
	void AnimNotify_SpawnEffect();

	UFUNCTION()
	void AnimNotify_ChangeCamera();

	UFUNCTION()
	void AnimNotify_JumpEnd();

	UFUNCTION()
	void AnimNotify_SpawnFresnel();

	UFUNCTION()
	void AnimNotify_ResetFresnel();

	UFUNCTION()
	void AnimNotify_Delay();

	UFUNCTION()
	void AnimNotify_Dash();

	UPROPERTY(BlueprintReadOnly)
	class AValkyrie* mCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	class UCharacterMovementComponent* mMovementComp;

	FOnNextAttackCheckDelegate	mOnNextAttackCheck;
	FOnAttackEndDelegate		mOnAttackEnd;
	FOnAttackCheckStartDelegate	mOnAttackCheckStart;
	FOnLaunchDelegate			mOnLaunch;
	FOnSkillEndDelegate			mSkillEnd;
	FOnChangeCameraDelegate		mChangeCamera;
	FOnJumpEndDelegate			mOnJumpEnd;
	FOnSpawnFresnelDelegate		mSpawnFresnel;
	FOnResetFresnelDelegate		mResetFresnel;
	FOnDelayDelegate			mOnDelay;

	bool GetIsJumpAttackEnd() { return mIsJumpAttackEnd; }
	bool GetIsCrouch() { return mIsCrouch; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float mGroundSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float mDirection = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float mCrouchValue = 0.f;



	UPROPERTY(VisibleAnywhere, Category = "DistanceMatching", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector mAccelation;

	UPROPERTY(VisibleAnywhere, Category = "DistanceMatching", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool mIsAccel;

	UPROPERTY(VisibleAnywhere, Category = "DistanceMatching", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector mVelocity;

	UPROPERTY(VisibleAnywhere, Category = "DistanceMatching", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool mIsVelocity;

	UPROPERTY(VisibleAnywhere, Category = "DistanceMatching", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float mDistanceLastUpdate;

	UPROPERTY(VisibleAnywhere, Category = "DistanceMatching", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector mCurrentWorldLocation;

	UPROPERTY(VisibleAnywhere, Category = "DistanceMatching", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float mCurrentSpeed;

	UPROPERTY(VisibleAnywhere, Category = "PoseWarping", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float mLocalVelocityDirectionAngle;

	UPROPERTY(VisibleAnywhere, Category = "PoseWarping", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EMoveDirection mMoveDirection;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool	mIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool	mIsCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EActionState	mActionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool	mIsJumpAttackEnd;

private:

	
	UPROPERTY()
	FTimerHandle	mTimer;
};
