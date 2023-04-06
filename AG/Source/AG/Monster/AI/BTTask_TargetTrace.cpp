// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetTrace.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"


UBTTask_TargetTrace::UBTTask_TargetTrace()
{
	NodeName = TEXT("TargetTrace");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TargetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//---------------
	// MonsterAIController, Monster, BB�� Target �� ���´�.
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
	// Target �� ������ Idle, ������ Target �� �Ѿư���.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		return EBTNodeResult::Failed;
	}

	// else.
	UAIBlueprintHelperLibrary::SimpleMoveToActor(controller, target);

	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::CHASE);


	// ���Ͱ� Ÿ�ٿ� ������ ������ �� Task�� ���������� ���ϰ� �Ѵ�.
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_TargetTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_TargetTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//---------------
	// MonsterAIController, Monster(Info, AnimInst), BB�� Target �� ���´�.
	//---------------
	AMonsterAIController* controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(controller))
	{
		// FinishLatentTask(): Task�� ������ �����Ų��.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	AMonster* monster = Cast<AMonster>(controller->GetPawn());

	if (!IsValid(monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	const FMonsterInfo& monsterInfo = monster->GetMonsterInfo();

	UMonsterAnimInstance* monsterAnimInst = monster->GetMonsterAnimInst();

	if (!IsValid(monsterAnimInst))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ACharacter* target = Cast<ACharacter>(controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	


	//---------------
	// Target �� ������ Idle/Task ����, ������ Target �� �Ѿư���.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		// Task�� �����Ų��.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}


	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();

	float distance = (monsterPosition - targetPosition).Size();

	if (distance <= monsterInfo.attackDistance)
	{
		controller->StopMovement();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_TargetTrace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}