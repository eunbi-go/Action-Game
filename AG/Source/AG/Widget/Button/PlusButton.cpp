// Fill out your copyright notice in the Description page of Project Settings.


#include "PlusButton.h"
#include "AGButton.h"

void UPlusButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	mButton = Cast<UAGButton>(GetWidgetFromName(TEXT("UI_AGButton")));
}

void UPlusButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlusButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}