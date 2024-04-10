// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAGAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag mAttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText mAttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText mAttributeDescription = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float mAttributeValue = 0.f;
};



/**
 * 
 */
UCLASS()
class AG_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FAGAttributeInfo FindAttributeInfoByTag(const FGameplayTag& AttributeTag);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAGAttributeInfo> mAttributeInfosArray;
};
