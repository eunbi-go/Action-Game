// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "../../AbilitySystem/Data/AttributeInfo.h"
#include "AGAttributeValueButtonRow.generated.h"

class UAttributeValueRow;
class UPlusButton;

/**
 * 
 */
UCLASS()
class AG_API UAGAttributeValueButtonRow : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void PlusButtonClicked();

	UPROPERTY(EditAnywhere)
	UAttributeValueRow* mRow;

	UPROPERTY()
	UPlusButton* mPlusButton;

	// 에디터에서 추가 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> mEffect;
};
