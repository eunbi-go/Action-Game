// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"


UENUM(BlueprintType)
enum class MONSTER_MOTION : uint8
{
	IDLE, PATROL, CHASE, ATTACK, DEATH, END
};


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

public:
	void Hit();
	void SetHitDirection(FString _value);

	
public:
	void SetMonsterMotionType(MONSTER_MOTION _motion) { mMonsterMotionType = _motion; }

public:
	MONSTER_MOTION GetMonsterMotionType() { return mMonsterMotionType; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	MONSTER_MOTION	mMonsterMotionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mHitMontageArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mHitAdditive;

	// front, back, left, right.
	int32		mHitMontageIndex;
	FString		mHitDirection;
};
