// Fill out your copyright notice in the Description page of Project Settings.


#include "AGButton.h"

void UAGButton::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAGButton::NativeConstruct()
{
	Super::NativeConstruct();
	mButton = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
	mSizeBox = Cast<USizeBox>(GetWidgetFromName(TEXT("SizeBox_Root")));
	mText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ButtonName")));

	mButtonState = BUTTON_STATE::UNCLICKED;
}

void UAGButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
