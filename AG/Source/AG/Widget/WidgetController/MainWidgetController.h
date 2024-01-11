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

public:
	virtual void BroadcastInitValues() override;
	
	FOnHpChange mOnHpChange;

	FOnMaxHpChange mOnMaxHpChange;
};
