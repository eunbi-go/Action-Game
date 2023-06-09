// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"


UCLASS()
class AG_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;


public:
	void AddItemByKey(EITEM_ID _id);

	UFUNCTION()
	void UseItem(EITEM_ID _id, class APlayerCharacter* userCharacter);
	
	UFUNCTION(BlueprintCallable)
	void Clicked(UObject* item);


public:
	TMap<EITEM_ID, class UItemData*> GetItemMap() 
	{
		return mItemMap; 
	}

	class UTileView* GetTileView() 
	{
		return mTileView; 
	}


private:
	class UTileView* mTileView;
	TMap<EITEM_ID, class UItemData*> mItemMap;
};
