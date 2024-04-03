// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGWidgetController.h"
#include "MainWidgetController.generated.h"

class UMainWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag messageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText message = FText();

	// 메세지를 띄울 화면
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMainWidget> messageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* image = nullptr;
};


/**
 * 
 */
UCLASS()
class AG_API UMainWidgetController : public UAGWidgetController
{
	GENERATED_BODY()
		//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHpChange, float, newHp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHpChange, float, newMaxHp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMpChange, float, newMp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxMpChange, float, newMaxMp);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinChange, float, newCoin);

public:
	virtual void BroadcastInitValues() override;
	
	virtual void BindCallbacksToDependecies() override;

	FOnHpChange mOnHpChange;
	FOnMaxHpChange mOnMaxHpChange;
	FOnMpChange mOnMpChange;
	FOnMaxMpChange mOnMaxMpChange;
	FOnCoinChange	mOnCoinChange;

protected:
	void HpChange(const FOnAttributeChangeData& data) const;
	void MaxHpChange(const FOnAttributeChangeData& data) const;
	void MpChange(const FOnAttributeChangeData& data) const;
	void MaxMpChange(const FOnAttributeChangeData& data) const;
	void CoinChange(const FOnAttributeChangeData& data) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data");
	UDataTable* mMessageDataTable;
};
