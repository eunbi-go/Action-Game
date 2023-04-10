// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsEnd.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UBTDecorator_IsEnd : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsEnd();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
