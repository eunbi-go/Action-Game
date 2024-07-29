// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Engine/DataAsset.h"
#include "AGInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FAGInputAction
{
	GENERATED_BODY()

	// InputAction은 GameplayTag와 연결되어야 한다.
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* mInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag mInputTag = FGameplayTag();
};


/**
 * 
 */
UCLASS()
class AG_API UAGInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAGInputAction> mAbilityInputActions;
};
