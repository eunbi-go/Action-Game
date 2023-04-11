// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetHp(float _ratio);
	void SetHpRatio(float _ratio) { mHpTargetRatio = _ratio; }
	void SetMp(float _ratio);
	void SetMpRatio(float _ratio) { mMpTargetRatio = _ratio; }

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _deltaTime) override;


private:
	UProgressBar*	mHpBar;
	UProgressBar*	mMpBar;

	float			mHpTargetRatio;
	float			mHpRatio;

	float			mMpTargetRatio;
};
