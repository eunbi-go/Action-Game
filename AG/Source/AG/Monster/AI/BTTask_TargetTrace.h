// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetTrace.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UBTTask_TargetTrace : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TargetTrace();


protected:
	// Task ������ �� 1�� ȣ��.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	// Task �ߴܵ� �� ȣ��.
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	// bNotifyTick�� true�� ���, ��� ȣ��.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	// Task ����� �� ȣ��.
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);
};
