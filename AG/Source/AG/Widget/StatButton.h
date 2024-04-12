// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "StatButton.generated.h"

class UStatWidget;
/**
 * 
 */
UCLASS()
class AG_API UStatButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void StatButtonClicked();

public:


private:
	class UButton* mButton;
	class USizeBox* mSizeBox;
	class UTextBlock* mText;
	UPROPERTY(EditAnywhere)
	class UStatWidget* mStatWidget;
};
