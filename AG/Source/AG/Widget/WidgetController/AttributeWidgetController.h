// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGWidgetController.h"
#include "../../AbilitySystem/Data/AttributeInfo.h"
#include "AttributeWidgetController.generated.h"




/**
 * 
 */

class UAttributeInfo;
struct FAGAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAGAttributeInfo&, Info);


UCLASS(BlueprintType)
class AG_API UAttributeWidgetController : public UAGWidgetController
{
	GENERATED_BODY()
	
public:
	UAttributeWidgetController();
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependecies() override;
	
	UPROPERTY(BlueprintAssignable)
	FAttributeInfoSignature mAttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	UAttributeInfo* mAttributeInfo;
};




