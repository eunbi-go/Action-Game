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

/**
 * 
 */
UCLASS()
class AG_API UValkyrieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
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

	UPROPERTY(BlueprintReadOnly)
	class AValkyrie* mCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* mMovementComp;

	FOnNextAttackCheckDelegate	mOnNextAttackCheck;
	FOnAttackEndDelegate		mOnAttackEnd;
	FOnAttackCheckStartDelegate	mOnAttackCheckStart;
	FOnLaunchDelegate			mOnLaunch;
	FOnSkillEndDelegate			mSkillEnd;
	FOnChangeCameraDelegate		mChangeCamera;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mGroundSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mDirection = 0.f;

	

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool	mIsInAir;
};
