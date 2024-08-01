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
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _deltaTime) override;


	UFUNCTION()
	void SetNewHp(float newHp);
	UFUNCTION()
	void SetNewMaxHp(float newMaxHp);

	UFUNCTION()
	void SetNewMp(float newMp);
	UFUNCTION()
	void SetNewMaxMp(float newMaxMp);
	
	UFUNCTION()
	void SetNewCoin(float newCoin)
	{
		mNewCoin = newCoin;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* widgetController);

	UPROPERTY(BlueprintReadOnly)
	UObject* mWidgetController;

	





private:
	UPROPERTY()
	FTimerHandle mTimerHandle1;

	UPROPERTY()
	FTimerHandle mTimerHandle2;

	UPROPERTY()
	FTimerHandle mTimerHandleHp1;

	UPROPERTY()
	FTimerHandle mTimerHandleHp2;

	UPROPERTY()
	FTimerHandle mTimerHandleMaxHp11;

	UPROPERTY()
	FTimerHandle mTimerHandleMaxHp2;

	UPROPERTY()
	FTimerHandle mTimerHandleMaxMp11;

	UPROPERTY()
	FTimerHandle mTimerHandleMaxMp2;

	UProgressBar*	mHpBar;
	UProgressBar*	mMpBar;
	UProgressBar*	mGhostMpBar;
	UProgressBar*	mGhostHpBar;
	class UTextBlock* mCoinTxt;

	float			mNewHp;
	float			mNewMp;
	float			mNewMaxHp;
	float			mNewMaxMp;

	float			mGhostMpTarget;

	int				mNewCoin;
	bool			mIsInitMp = false;
	bool			mIsInitHp = false;
	bool			mIsInitMaxHp = false;
	bool			mIsInitMaxMp = false;
};
