// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "../Player/CharacterStatComponent.h"
#include "InventoryWidget.h"
#include "ItemQuickSlot.h"
#include "BossInfoWidget.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayerInfo = Cast<UPlayerInfoWidget>(GetWidgetFromName(FName(TEXT("PlayerInfo"))));
	mInventory = Cast<UInventoryWidget>(GetWidgetFromName(FName(TEXT("Inventory"))));
	mItemQuickSlot = Cast<UItemQuickSlot>(GetWidgetFromName(FName(TEXT("ItemQuickSlot"))));
	mBossInfo = Cast<UBossInfoWidget>(GetWidgetFromName(FName(TEXT("BossInfo"))));

	mBossInfo->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo, _deltaTime);
}

void UMainWidget::SetCharacterStat(UCharacterStatComponent* _characterStat)
{
	if (_characterStat == nullptr)
		return;

	mCurrentStat = _characterStat;


	mCurrentStat->mHpChange.AddUObject(this, &UMainWidget::UpdateHp);
	UpdateHp();

	mCurrentStat->mMpChange.AddUObject(this, &UMainWidget::UpdateMp);
	UpdateMp();

	mCurrentStat->mCoinChange.AddUObject(this, &UMainWidget::UpdateCoin);
	UpdateCoin();

	UpdateBossHp(1.f, 1.f);
}

void UMainWidget::UpdateHp()
{
	mPlayerInfo->SetHpRatio(mCurrentStat->GetHpRatio());
}

void UMainWidget::UpdateMp()
{
	mPlayerInfo->SetMpRatio(mCurrentStat->GetMpRatio());
}

void UMainWidget::UpdateCoin()
{
	mPlayerInfo->SetCoin(mCurrentStat->GetCurrentInfo().gold);
}

void UMainWidget::UpdateBossHp(float _hp, float _maxHp)
{
	mBossInfo->SetHpRatio(_hp / _maxHp);
}

void UMainWidget::BossInfoOnOff(bool _value)
{
	if (_value)
		mBossInfo->SetVisibility(ESlateVisibility::Visible);
	else
		mBossInfo->SetVisibility(ESlateVisibility::Hidden);
}

void UMainWidget::SetHp(float _ratio)
{
	mPlayerInfo->SetHp(_ratio);
}

void UMainWidget::SetMp(float _ratio)
{
	mPlayerInfo->SetMp(_ratio);
}
