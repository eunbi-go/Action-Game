// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"

void UPlayerInfoWidget::SetHp(float _ratio)
{
	mHpBar->SetPercent(_ratio);
}

void UPlayerInfoWidget::SetMp(float _ratio)
{
	mMpBar->SetPercent(_ratio);
}

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("Hp_Bar"))));
	mMpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("Mp_Bar"))));
}

void UPlayerInfoWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo, _deltaTime);
}
