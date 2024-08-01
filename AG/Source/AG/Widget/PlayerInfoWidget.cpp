// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "WidgetController/MainWidgetController.h"


void UPlayerInfoWidget::SetNewHp(float newHp)
{
	if (!mIsInitHp && newHp)
	{
		mIsInitHp = true;
		mNewHp = newHp;
		mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(newHp, mNewMaxHp));
		return;
	}

	if (newHp >= mNewHp)
	{
		// ghost bar 먼저 늘어남
		mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(newHp, mNewMaxHp));

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandleHp1,
			FTimerDelegate::CreateLambda([this, newHp]() {
				mNewHp = newHp;
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandleHp1);
				}), 0.2f, false);

	}
	else
	{
		// bar 먼저 줄어들고, 그 다음에 ghost bar 줄어듦
		mNewHp = newHp;

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandleHp2,
			FTimerDelegate::CreateLambda([this, newHp]() {
				mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(newHp, mNewMaxHp));
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandleHp2);
				}), 0.3f, false);
	}
}

void UPlayerInfoWidget::SetNewMaxHp(float newMaxHp)
{
	if (!mIsInitMaxHp && newMaxHp)
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
			mTimerHandleMaxHp11,
			FTimerDelegate::CreateLambda([this, newMaxHp]() {
				mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp));
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandleMaxHp11);
				}), 0.3f, false);
	}
	else
	{
		// ghost bar 먼저 늘어남
		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandleMaxHp2,
			FTimerDelegate::CreateLambda([this, newMaxHp]() {
				mNewMaxHp = newMaxHp;
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandleMaxHp2);
				}), 0.2f, false);

		mGhostHpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp));
	}
}

void UPlayerInfoWidget::SetNewMp(float newMp)
{
	if (!mIsInitMp && newMp)
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
			mTimerHandle1,
			FTimerDelegate::CreateLambda([this, newMp]() {
				mNewMp = newMp;
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandle1);
				}), 0.2f, false);

	}
	else
	{
		// bar 먼저 줄어들고, 그 다음에 ghost bar 줄어듦
		mNewMp = newMp;

		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandle2,
			FTimerDelegate::CreateLambda([this, newMp]() {
				mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(newMp, mNewMaxMp));
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandle2);
				}), 0.3f, false);
	}
}

void UPlayerInfoWidget::SetNewMaxMp(float newMaxMp)
{
	if (!mIsInitMaxMp && newMaxMp)
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
			mTimerHandleMaxMp11,
			FTimerDelegate::CreateLambda([this, newMaxMp]() {
				mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp));
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandleMaxMp11);
				}), 0.3f, false);

	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			mTimerHandleMaxMp2,
			FTimerDelegate::CreateLambda([this, newMaxMp]() {
				mNewMaxMp = newMaxMp;
				GetWorld()->GetTimerManager().ClearTimer(mTimerHandleMaxMp2);
				}), 0.2f, false);

		// ghost bar 먼저 늘어남
		mGhostMpBar->SetPercent(UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp));
	}
}

void UPlayerInfoWidget::SetWidgetController(UObject* widgetController)
{
	mWidgetController = widgetController;
	Cast<UMainWidgetController>(mWidgetController)->mOnMaxHpChange.AddDynamic(this, &UPlayerInfoWidget::SetNewMaxHp);
	Cast<UMainWidgetController>(mWidgetController)->mOnHpChange.AddDynamic(this, &UPlayerInfoWidget::SetNewHp);
	Cast<UMainWidgetController>(mWidgetController)->mOnMaxMpChange.AddDynamic(this, &UPlayerInfoWidget::SetNewMaxMp);
	Cast<UMainWidgetController>(mWidgetController)->mOnMpChange.AddDynamic(this, &UPlayerInfoWidget::SetNewMp);
	Cast<UMainWidgetController>(mWidgetController)->mOnCoinChange.AddDynamic(this, &UPlayerInfoWidget::SetNewCoin);
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
	//float fq = UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp);
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("x: %f, hp %f, : maxhp : %f"), fq, mNewHp, mNewMaxHp));
	
	mHpBar->SetPercent(FMath::FInterpTo(mHpBar->Percent, UKismetMathLibrary::SafeDivide(mNewHp, mNewMaxHp), _deltaTime, 5.f));
	mMpBar->SetPercent(FMath::FInterpTo(mMpBar->Percent, UKismetMathLibrary::SafeDivide(mNewMp, mNewMaxMp), _deltaTime, 5.f));
	mCoinTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), mNewCoin)));
}