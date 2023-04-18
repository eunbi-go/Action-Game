// Fill out your copyright notice in the Description page of Project Settings.


#include "BossInfoWidget.h"

void UBossInfoWidget::SetHp(float _ratio)
{
	mHpBar->SetPercent(_ratio);
}

void UBossInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("Hp_Bar"))));

	mHpTargetRatio = 1.f;
	mHpRatio = 1.f;
}

void UBossInfoWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo, _deltaTime);

	mHpBar->SetPercent(FMath::FInterpTo(mHpBar->Percent, mHpTargetRatio, _deltaTime, 5.f));
}
