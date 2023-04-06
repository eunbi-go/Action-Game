// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolWait.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UBTTask_PatrolWait : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PatrolWait();


protected:
	// Task 실행할 때 1번 호출.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	// Task 중단될 때 호출.
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	// bNotifyTick이 true일 경우, 계속 호출.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	// Task 종료될 때 호출.
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mWaitTime;

	float	mProgressTime;
};
