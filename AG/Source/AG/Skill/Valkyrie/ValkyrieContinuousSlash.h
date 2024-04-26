// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGSkillActor.h"
#include "ValkyrieContinuousSlash.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieContinuousSlash : public AAGSkillActor
{
	GENERATED_BODY()
public:
	AValkyrieContinuousSlash();
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnEffect() override;
	virtual void SkillEnd() override;

	void InputPressed();
	void Notify_SlashEnable();

protected:
	virtual void BeginPlay() override;


	int32	mSlashSkillIndex = -1;
	bool	mIsNextSlashEnable = false;
	bool	mIsNextSlashInput = false;
	bool	mIsSlash = false;
};
