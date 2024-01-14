// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGWidgetController.h"
#include "MainWidgetController.generated.h"



/**
 * 
 */
UCLASS()
class AG_API UMainWidgetController : public UAGWidgetController
{
	GENERATED_BODY()
		//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHpChange, float, newHp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHpChange, float, newMaxHp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMpChange, float, newMp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxMpChange, float, newMaxMp);

public:
	virtual void BroadcastInitValues() override;
	
	virtual void BindCallbacksToDependecies() override;

	FOnHpChange mOnHpChange;
	FOnMaxHpChange mOnMaxHpChange;
	FOnMpChange mOnMpChange;
	FOnMaxMpChange mOnMaxMpChange;

protected:
	void HpChange(const FOnAttributeChangeData& data) const;
	void MaxHpChange(const FOnAttributeChangeData& data) const;
	void MpChange(const FOnAttributeChangeData& data) const;
	void MaxMpChange(const FOnAttributeChangeData& data) const;
};
