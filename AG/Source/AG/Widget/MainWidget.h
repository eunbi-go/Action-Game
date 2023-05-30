// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "../BasicInfo.h"

#include "PlayerInfoWidget.h"

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


public:
	void SetCharacterStat(class UCharacterStatComponent* _characterStat);
	
	void UpdateHp();
	void UpdateMp();
	void UpdateCoin();
	void InitCoin();
	
	void UpdateBossHp(float _hp, float _maxHp);

	UFUNCTION()
	void UseItem(EITEM_ID id, class APlayerCharacter* userCharacter);


public:
	void BossInfoOnOff(bool _value);



public:
	void SetHp(float _ratio);
	void SetMp(float _ratio);
	

public:
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
	TWeakObjectPtr<class UCharacterStatComponent> mCurrentStat;


	class UInventoryWidget* mInventory;
	class UItemQuickSlot*	mItemQuickSlot;
public:
	class UBossInfoWidget* mBossInfo;
};
