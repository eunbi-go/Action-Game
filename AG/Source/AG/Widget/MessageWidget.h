// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _deltaTime) override;

	UFUNCTION()
	void SetMessage(const FUIWidgetRow Row);

private:
	UImage* mImage;
	class UTextBlock* mMessage;

	UPROPERTY()
	FTimerHandle	mTimer;
};
