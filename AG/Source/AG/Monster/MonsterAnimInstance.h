// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"





UCLASS()
class AG_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMonsterAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_DeathEnd();

	UFUNCTION()
	void AnimNotify_HitEnd();

	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_AttackCheck();

	UFUNCTION()
	void AnimNotify_SkillEnd();

	UFUNCTION()
		void AnimNotify_Skill1();

	UFUNCTION()
	void AnimNotify_Skill3();

	UFUNCTION()
	void AnimNotify_Skill4();


public:
	UFUNCTION()
		void AnimNotify_PlayRataDown();

	UFUNCTION()
		void AnimNotify_PlayRataReset();


public:
	void Hit();
	void SetHitDirection(FString _value);
	
	void Attack();
	
public:
	void SetMonsterMotionType(MONSTER_MOTION _motion);

public:
	MONSTER_MOTION GetMonsterMotionType() { return mMonsterMotionType; }
	bool GetIsHit() { return mIsHit; }
	bool GetIsSkillEnd() { return mIsSkillEnd; }


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	MONSTER_MOTION	mMonsterMotionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mHitMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAnimMontage*	mAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mSkillMontageArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mHitAdditive;

	// front, back, left, right.
	int32		mHitMontageIndex;
	FString		mHitDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool		mIsHit;
	bool		mIsSkillEnd;
	int32		mCurSkillMontagIndex;
};
