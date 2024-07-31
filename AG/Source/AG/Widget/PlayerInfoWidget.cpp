// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "WidgetController/MainWidgetController.h"


void UPlayerInfoWidget::SetNewHp(float newHp)
{
	if (!mIsInitHp)
	{
		mIsInitHp = true;
		mNewHp = newHp;
		mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(newHp, mNewMaxHp));
		return;
	}

	if (newHp >= mNewHp)
	{
		// ghost bar 먼저 늘어남
		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newHp]() {
				mNewHp = newHp;
				}), 0.2f, false);

		mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(newHp, mNewMaxHp));
	}
	else
	{
		// bar 먼저 줄어들고, 그 다음에 ghost bar 줄어듦
		mNewHp = newHp;

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newHp]() {
				mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(newHp, mNewMaxHp));
				}), 0.3f, false);
	}
}

void UPlayerInfoWidget::SetNewMaxHp(float newMaxHp)
{
	if (!mIsInitMaxHp)
	{
		mIsInitMaxHp = true;
		mNewMaxHp = newMaxHp;
		mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp));
		return;
	}

	if (newMaxHp >= mNewMaxHp)
	{
		// bar 먼저 줄어들고, 그 다음에 ghost bar 줄어듦
		mNewMaxHp = newMaxHp;

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newMaxHp]() {
				mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp));
				}), 0.3f, false);
	}
	else
	{
		// ghost bar 먼저 늘어남
		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newMaxHp]() {
				mNewMaxHp = newMaxHp;
				}), 0.2f, false);

		mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp));
	}
}

void UPlayerInfoWidget::SetNewMp(float newMp)
{
	if (!mIsInitMp)
	{
		mIsInitMp = true;
		mNewMp = newMp;
		mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(newMp, mNewMaxMp));
		return;
	}

	if (newMp >= mNewMp)
	{
		// ghost bar 먼저 늘어남
		mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(newMp, mNewMaxMp));

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newMp]() {
				mNewMp = newMp;
				}), 0.2f, false);

	}
	else
	{
		// bar 먼저 줄어들고, 그 다음에 ghost bar 줄어듦
		mNewMp = newMp;

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newMp]() {
				mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(newMp, mNewMaxMp));
				}), 0.3f, false);
	}
}

void UPlayerInfoWidget::SetNewMaxMp(float newMaxMp)
{
	if (!mIsInitMaxMp)
	{
		mIsInitMaxMp = true;
		mNewMaxMp = newMaxMp;
		mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp));
		return;
	}

	if (newMaxMp >= mNewMaxMp)
	{
		// bar 먼저 줄어들고, 그 다음에 ghost bar 줄어듦
		mNewMaxMp = newMaxMp;

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newMaxMp]() {
				mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp));
				}), 0.3f, false);

	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			FTimerDelegate::CreateLambda([this, newMaxMp]() {
				mNewMaxMp = newMaxMp;
				}), 0.2f, false);

		// ghost bar 먼저 늘어남
		mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp));
	}
}

void UPlayerInfoWidget::SetWidgetController(UObject* widgetController)
{
	mWidgetController = widgetController;

}

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("Hp_Bar"))));
	mMpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("Mp_Bar"))));
	mGhostMpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("MpBar_Ghost"))));
	mGhostHpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HpBar_Ghost"))));
	mCoinTxt = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("CoinText"))));
}

void UPlayerInfoWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo , _deltaTime);
	float fq = UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp);

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("x: %f, hp %f, : maxhp : %f"), fq, mNewHp, mNewMaxHp));

	mHpBar->SetPercent(FMath::FInterpTo(mHpBar->Percent, UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp), _deltaTime, 5.f));
	mMpBar->SetPercent(FMath::FInterpTo(mMpBar->Percent, UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp), _deltaTime, 5.f));
	mCoinTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), mNewCoin)));
}