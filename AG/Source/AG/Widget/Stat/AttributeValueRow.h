// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "../../AbilitySystem/Data/AttributeInfo.h"
#include "AttributeValueRow.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UAttributeValueRow : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void ValueChanged(const FAGAttributeInfo& Info);

	UPROPERTY()
	UTextBlock* mAttributeName;

	UPROPERTY()
	UTextBlock* mAttributeValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag mTag;
};
