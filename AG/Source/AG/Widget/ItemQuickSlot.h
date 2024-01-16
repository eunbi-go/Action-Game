// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "ItemQuickSlot.generated.h"



/**
 * 
 */
UCLASS()
class AG_API UItemQuickSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
	
	void UseItem(int32 _index, ACharacter* userObject);

public:
	void AddItemToQuickSlot(class UItemData* _itemData);

protected:
	class UListView* mListView;
	int32	mQuickSlotSize;
};
