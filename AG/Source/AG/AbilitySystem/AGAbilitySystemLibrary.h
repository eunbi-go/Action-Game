// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AGAbilitySystemLibrary.generated.h"

class UMainWidgetController;
class UAttributeWidgetController;

/**
 * 
 */
UCLASS()
class AG_API UAGAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AGAbilitySystemLibrary|WidgetController")
	static UMainWidgetController* GetMainWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AGAbilitySystemLibrary|WidgetController")
	static UAttributeWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);
};
