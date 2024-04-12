// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlusButton.generated.h"

class UAGButton;

/**
 * 
 */
UCLASS()
class AG_API UPlusButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY()
	UAGButton* mButton;
};
