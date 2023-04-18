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

public:
	void AddItemToQuickSlot(class UItemData* _itemData);
	//void AddItemTo(UItemData* i){mListView.AddItem(i);}
protected:
	class UListView* mListView;
};
