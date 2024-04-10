// Fill out your copyright notice in the Description page of Project Settings.


#include "StatWidget.h"
#include "../Button/AGButton.h"
#include "../Button/CloseButton.h"

void UStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mCloseButton = Cast<UCloseButton>(GetWidgetFromName(FName(TEXT("CloseButton"))));
	mCloseButton->mButton->GetButton()->OnClicked.AddDynamic(this, &UStatWidget::CloseButtonClicked);
}

void UStatWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UStatWidget::CloseButtonClicked()
{
	PrintViewport(3.f, FColor::Yellow, FString("UStatWidget Close Button"));

	mOnCloseButtonClickedDelegate.Broadcast();
	RemoveFromParent();
}
