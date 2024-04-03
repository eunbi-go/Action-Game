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
	TSubclassOf<class UMainWidget> messageWidget;

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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChange, float, NewValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

public:
	virtual void BroadcastInitValues() override;
	
	virtual void BindCallbacksToDependecies() override;

	FOnAttributeChange mOnHpChange;
	FOnAttributeChange mOnMaxHpChange;
	FOnAttributeChange mOnMpChange;
	FOnAttributeChange mOnMaxMpChange;
	FOnAttributeChange	mOnCoinChange;

	FMessageWidgetRowSignature mOnMessageWidgetRowChange;
};