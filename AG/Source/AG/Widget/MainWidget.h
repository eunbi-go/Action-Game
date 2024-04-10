// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "../BasicInfo.h"

#include "PlayerInfoWidget.h"
#include "WidgetController/MainWidgetController.h"

#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _deltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UMainWidgetController* widgetController);

	// �ý��ۿ��� �����͸� �����ͼ� ��� ������ ��ε�ĳ�����Ѵ�.
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UObject* mWidgetController;

	void UpdateBossHp(float _hp, float _maxHp);
	void BossInfoOnOff(bool _value);

	UFUNCTION()
	void Tmp(const FUIWidgetRow Row);

	class UInventoryWidget* GetInventoryWidget() 
	{ 
		return mInventory; 
	}

	class UItemQuickSlot* GetItemQuickSlot() 
	{
		return mItemQuickSlot; 
	}

private:
	UPlayerInfoWidget*	mPlayerInfo;


	class UInventoryWidget* mInventory;
	class UItemQuickSlot*	mItemQuickSlot;
	class UMessageWidget* mMessage;
	class UStatMenuButton* mStatMenuButton;
public:
	class UBossInfoWidget* mBossInfo;
};
