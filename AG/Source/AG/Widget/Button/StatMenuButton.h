// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "StatMenuButton.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UStatMenuButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void ButtonClicked();

	class UAGButton* mButton;

	UPROPERTY(EditAnywhere)
	class UStatWidget* mStatWidget;
};
