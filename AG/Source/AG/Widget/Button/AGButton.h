// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "AGButton.generated.h"

UENUM(BlueprintType)
enum class BUTTON_STATE : uint8
{
	CLICKED, UNCLICKED
};

/**
 * 
 */
UCLASS()
class AG_API UAGButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



	UButton* mButton;
	USizeBox* mSizeBox;
	UTextBlock* mText;

	BUTTON_STATE	mButtonState;

	float	mButtonWidth;
	float	mButtonHeight;

public:
	UButton* GetButton() { return mButton; }
};
