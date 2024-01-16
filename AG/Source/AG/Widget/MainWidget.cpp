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