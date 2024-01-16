// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "../Player/CharacterStatComponent.h"
#include "../Player/PlayerCharacter.h"
#include "InventoryWidget.h"
#include "ItemQuickSlot.h"
#include "BossInfoWidget.h"
#include "WidgetController/MainWidgetController.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayerInfo = Cast<UPlayerInfoWidget>(GetWidgetFromName(FName(TEXT("PlayerInfo"))));
	mInventory = Cast<UInventoryWidget>(GetWidgetFromName(FName(TEXT("Inventory"))));
	mItemQuickSlot = Cast<UItemQuickSlot>(GetWidgetFromName(FName(TEXT("ItemQuickSlot"))));
	mBossInfo = Cast<UBossInfoWidget>(GetWidgetFromName(FName(TEXT("BossInfo"))));

	mBossInfo->SetVisibility(ESlateVisibility::Hidden);
	mInventory->SetVisibility(ESlateVisibility::Hidden);

	Cast<UMainWidgetController>(mWidgetController)->mOnMaxHpChange.AddDynamic(mPlayerInfo, &UPlayerInfoWidget::SetNewMaxHp);
	Cast<UMainWidgetController>(mWidgetController)->mOnHpChange.AddDynamic(mPlayerInfo, &UPlayerInfoWidget::SetNewHp);
	Cast<UMainWidgetController>(mWidgetController)->mOnMaxMpChange.AddDynamic(mPlayerInfo, &UPlayerInfoWidget::SetNewMaxMp);
	Cast<UMainWidgetController>(mWidgetController)->mOnMpChange.AddDynamic(mPlayerInfo, &UPlayerInfoWidget::SetNewMp);
}

void UMainWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo, _deltaTime);
}

void UMainWidget::SetWidgetController(UMainWidgetController* widgetController)
{
	mWidgetController = widgetController;
}

void UMainWidget::SetCharacterStat(UCharacterStatComponent* _characterStat)
{
	if (_characterStat == nullptr)
		return;

	mCurrentStat = _characterStat;


	//mCurrentStat->mHpChange.AddUObject(this, &UMainWidget::UpdateHp);
	//UpdateHp();

	mCurrentStat->mMpChange.AddUObject(this, &UMainWidget::UpdateMp);
	UpdateMp();

	mCurrentStat->mCoinChange.AddUObject(this, &UMainWidget::UpdateCoin);
	UpdateCoin();

	/*mItemQuickSlot->mUseItems.AddDynamic(mInventory, &UInventoryWidget::UseItem);
	mItemQuickSlot->mUseItems.AddDynamic(this, &UMainWidget::UseItem);*/


	//PrintViewport(3.f, FColor::Yellow, TEXT("SetCharacterStat"));
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

void UMainWidget::InitCoin()
{
	mPlayerInfo->SetCoin(mCurrentStat->GetCurrentInfo().gold);
}

void UMainWidget::UpdateBossHp(float _hp, float _maxHp)
{
	mBossInfo->SetHpRatio(_hp / _maxHp);
}

void UMainWidget::UseItem(EITEM_ID id, APlayerCharacter* userCharacter)
{
	switch (id)
	{
	case EITEM_ID::POTION_HP_MIN:
		break;

	case EITEM_ID::POTION_HP_MAX:
		break;

	case EITEM_ID::POTION_MP_MIN:
		break;

	case EITEM_ID::POTION_MP_MAX:
		break;

	case EITEM_ID::COIN:
		break;

	case EITEM_ID::SWORD1:
		break;
	}
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