// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolWait.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"


UBTTask_PatrolWait::UBTTask_PatrolWait()
{
	NodeName = TEXT("PatrolWait");

	bNotifyTick = true;
	bNotifyTaskFinished = true;

	mWaitTime = 2.f;
	mProgressTime = 0.f;
}

EBTNodeResult::Type UBTTask_PatrolWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//---------------
	// MonsterAIController, Monster, BB의 Target 을 얻어온다.
	//---------------
	AMonsterAIController* controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(controller))
		return EBTNodeResult::Failed;


	AMonster* monster = Cast<AMonster>(controller->GetPawn());

	if (!IsValid(monster))
		return EBTNodeResult::Failed;


	UMonsterAnimInstance* monsterAnimInst = monster->GetMonsterAnimInst();

	if (!IsValid(monsterAnimInst))
		return EBTNodeResult::Failed;


	AActor* target = Cast<AActor>(controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));



	//---------------
	// Target 이 있으면 종료시킨다. 타겟 추적을 시작해야 한다.
	//---------------
	if (IsValid(target))
		return EBTNodeResult::Succeeded;

	

	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);
	controller->StopMovement();
	mProgressTime = 0.f;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PatrolWait::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_PatrolWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//---------------
	// MonsterAIController, Monster(Info, AnimInst), BB의 Target 을 얻어온다.
	//---------------
	AMonsterAIController* controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(controller))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	AMonster* monster = Cast<AMonster>(controller->GetPawn());

	if (!IsValid(monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	UMonsterAnimInstance* monsterAnimInst = monster->GetMonsterAnimInst();

	if (!IsValid(monsterAnimInst))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	AActor* target = Cast<AActor>(controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	
	if (IsValid(target))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	//---------------
	// Target 이 없으면 patrolWaitTime 을 증가시키고, 시간이 다 됐으면 종료.
	//---------------
	monster->SetPatrolWaitTime(DeltaSeconds);

	if (monster->GetPatrolWaitTime() >= mWaitTime)
	{
		monster->ClearPatrolWaitTime();
		PrintViewport(3.f, FColor::Yellow, TEXT("Wait Finish"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_PatrolWait::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
