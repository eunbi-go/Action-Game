// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGSkillActor.h"
#include "ValkyrieRange.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieRange : public AAGSkillActor
{
	GENERATED_BODY()
	
public:
	AValkyrieRange();
	virtual void Activate() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnEffect() override;
	virtual void SkillEnd() override;
	float GetPressingTime()
	{
		return mPressingTime;
	}
	void SetIsPress(bool NewValue)
	{
		mIsPress = NewValue;
	}
	void SetIsKeyReleased(bool NewValue)
	{
		mIsKeyReleased = NewValue;
	}
	bool GetIsPress()
	{
		return mIsPress;
	}
	bool GetIsKeyReleased()
	{
		return mIsKeyReleased;
	}

protected:
	virtual void BeginPlay() override;

	bool mInit = true;
	bool mIsPress = false;
	bool mIsKeyReleased = false;
	float mPressingTime = 0.f;
};
