// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "../BasicInfo.h"

#include "PlayerInfoWidget.h"

#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _deltaTime) override;


public:
	void SetHp(float _ratio);
	void SetMp(float _ratio);

private:
	UPlayerInfoWidget*	mPlayerInfo;
};
