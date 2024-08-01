// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "../Player/CharacterStatComponent.h"
#include "../Player/PlayerCharacter.h"
#include "InventoryWidget.h"
#include "ItemQuickSlot.h"
#include "BossInfoWidget.h"
#include "MessageWidget.h"
#include "Button/StatMenuButton.h"
#include "Stat/StatWidget.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayerInfo = Cast<UPlayerInfoWidget>(GetWidgetFromName(FName(TEXT("PlayerInfo"))));
	mInventory = Cast<UInventoryWidget>(GetWidgetFromName(FName(TEXT("Inventory"))));
	mItemQuickSlot = Cast<UItemQuickSlot>(GetWidgetFromName(FName(TEXT("ItemQuickSlot"))));
	mBossInfo = Cast<UBossInfoWidget>(GetWidgetFromName(FName(TEXT("BossInfo"))));
	mMessage = Cast<UMessageWidget>(GetWidgetFromName(FName(TEXT("Message"))));
	mStatMenuButton = Cast<UStatMenuButton>(GetWidgetFromName(FName(TEXT("StatMenuButton"))));

	mBossInfo->SetVisibility(ESlateVisibility::Hidden);
	mInventory->SetVisibility(ESlateVisibility::Hidden);
	mMessage->SetVisibility(ESlateVisibility::Hidden);

	mPlayerInfo->SetWidgetController(mWidgetController);


	Cast<UMainWidgetController>(mWidgetController)->mOnMessageWidgetRowChange.AddDynamic(mMessage, &UMessageWidget::SetMessage);
	Cast<UMainWidgetController>(mWidgetController)->BroadcastInitValues();
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

void UMainWidget::Tmp(const FUIWidgetRow Row)
{
	PrintViewport(2.f, FColor::Red, Row.message.ToString());
}
