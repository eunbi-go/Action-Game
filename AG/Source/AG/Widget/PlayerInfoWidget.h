// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



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
	
	void SetHpRatio(float _ratio)
	{ 
		mHpTargetRatio = _ratio; 
	}

	void SetMp(float _ratio);
	
	void SetMpRatio(float _ratio)
	{
		mMpTargetRatio = _ratio; 
	}

	void SetCoin(int32 _value) 
	{
		mCoin = _value; 
	}

	void InitCoin(int32 _value) 
	{
		mCoin = _value; 
	}


public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _deltaTime) override;







private:
	UProgressBar*	mHpBar;
	UProgressBar*	mMpBar;
	class UTextBlock* mCoinTxt;

	float			mHpTargetRatio;
	float			mHpRatio;

	float			mMpTargetRatio;

	int				mCoin;
};
