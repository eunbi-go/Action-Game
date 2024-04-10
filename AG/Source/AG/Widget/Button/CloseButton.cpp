// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseButton.h"
#include "AGButton.h"

void UCloseButton::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UCloseButton::NativeConstruct()
{
	Super::NativeConstruct();

	mButton = Cast<UAGButton>(GetWidgetFromName(FName(TEXT("UI_AGButton"))));
}

void UCloseButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}