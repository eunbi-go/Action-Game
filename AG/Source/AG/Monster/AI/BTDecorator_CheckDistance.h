// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckDistance.generated.h"

UENUM(BlueprintType)
enum class CHECK_TYPE : uint8
{
	ATTACK,		// 공격 거리 체크
	TRACE		// 추적 거리 체크
};

/**
 * 
 */
UCLASS()
class AG_API UBTDecorator_CheckDistance : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckDistance();


protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;




protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	CHECK_TYPE mCheckType;
};
