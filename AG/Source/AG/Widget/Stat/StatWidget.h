// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

class UCloseButton;

DECLARE_MULTICAST_DELEGATE(FOnCloseButtonClicked);
class UAttributeWidgetController;
class UAttributeValueRow;
class UAGAttributeValueButtonRow;
/**
 * 
 */
UCLASS()
class AG_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
	
	void SetAttributeTag();


	UFUNCTION()
	void CloseButtonClicked();
	UCloseButton* mCloseButton;
	FOnCloseButtonClicked mOnCloseButtonClickedDelegate;
	


	UPROPERTY()
	UObject* mWidgetController;
	void SetWidgetController(UAttributeWidgetController* widgetController);


	UScrollBox* mFirstAttributeScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mBlockPercentRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAGAttributeValueButtonRow* mDexterityRow;
};
