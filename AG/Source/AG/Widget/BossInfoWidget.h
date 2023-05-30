// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "BossInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UBossInfoWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetHp(float _ratio);
	void SetHpRatio(float _ratio) 
	{
		mHpTargetRatio = _ratio; 
	}

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _deltaTime) override;


private:
	class UProgressBar* mHpBar;

	float			mHpTargetRatio;
	float			mHpRatio;
};
