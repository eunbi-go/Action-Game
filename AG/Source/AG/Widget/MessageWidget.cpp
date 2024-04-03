// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageWidget.h"
#include "WidgetController/MainWidgetController.h"

void UMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mMessage = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Message"))));
	mImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("Icon"))));
}

void UMessageWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo, _deltaTime);
}

void UMessageWidget::SetMessage(const FUIWidgetRow Row)
{
	if (IsValid(Row.image))
		mImage->SetBrushFromTexture(Row.image);
		
	mMessage->SetText(Row.message);
}
