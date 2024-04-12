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


	UFUNCTION()
	void CloseButtonClicked();
	UCloseButton* mCloseButton;
	FOnCloseButtonClicked mOnCloseButtonClickedDelegate;
	


	UPROPERTY()
	UObject* mWidgetController;
	void SetWidgetController(UAttributeWidgetController* widgetController);


	//-------------------------------------------------
	// First Attributes
	//-------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAGAttributeValueButtonRow* mStrengthRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAGAttributeValueButtonRow* mDexterityRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAGAttributeValueButtonRow* mIntelligenceRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAGAttributeValueButtonRow* mResilienceRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mCoinRow;



	//-------------------------------------------------
	// Second Attributes
	//-------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mArmorRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mBlockPercentRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mCriticalPercentRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mCriticalResistanceRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mCriticalDamageRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mMaxHpRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributeValueRow* mMaxMpRow;
	
};
