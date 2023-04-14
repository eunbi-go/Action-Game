// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DT) override;

public:
	UFUNCTION(BlueprintCallable)
		void InitFromData(UObject* _Data);

private:
	class UImage* mIconImage;
	class UTextBlock* mCountTxt;
};
