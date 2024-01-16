// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "WidgetController/MainWidgetController.h"


void UPlayerInfoWidget::SetNewHp(float newHp)
{
	mNewHp = newHp;
}

void UPlayerInfoWidget::SetNewMaxHp(float newMaxHp)
{
	mNewMaxHp = newMaxHp;
}

void UPlayerInfoWidget::SetWidgetController(UObject* widgetController)
{
}

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("Hp_Bar"))));
	mMpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("Mp_Bar"))));
	mCoinTxt = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("CoinText"))));

	//mHpTargetRatio = 1.f;
	//mHpRatio = 1.f;

	//mMpTargetRatio = 1.f;

	//mCoin = 0;
}

void UPlayerInfoWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo , _deltaTime);
	float fq = UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp);

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("x: %f, hp %f, : maxhp : %f"), fq, mNewHp, mNewMaxHp));

	mHpBar->SetPercent(FMath::FInterpTo(mHpBar->Percent, UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp), _deltaTime, 5.f));
	mMpBar->SetPercent(FMath::FInterpTo(mMpBar->Percent, UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp), _deltaTime, 5.f));
	//mMpBar->SetPercent(FMath::FInterpTo(mMpBar->Percent, mMpTargetRatio, _deltaTime, 5.f));
	//mHpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp));
	mCoinTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), mNewCoin)));
}