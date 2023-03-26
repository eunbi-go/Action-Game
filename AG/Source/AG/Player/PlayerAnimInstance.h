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
	void ChangePlayMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*>	mIdleModeMontageArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mPlayModeValue;
	
	PLAYE_MODE	mCurPlayMode;
	PLAYER_STATE	mPlayerState;
};
