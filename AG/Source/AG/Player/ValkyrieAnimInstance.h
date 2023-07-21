// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Animation/AnimInstance.h"
#include "ValkyrieAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackCheckStartDelegate);

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

	UPROPERTY(BlueprintReadOnly)
	class AValkyrie* mCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* mMovementComp;

	FOnNextAttackCheckDelegate	mOnNextAttackCheck;
	FOnAttackEndDelegate		mOnAttackEnd;
	FOnAttackCheckStartDelegate	mOnAttackCheckStart;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mGroundSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float mDirection = 0.f;

	

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool	mIsInAir;
};
