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
	// Target 이 없으면 Idle -> Task 종료, 있으면 Target 을 향해 회전한 후, 쫓아간다.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);
		return EBTNodeResult::Failed;
	}

	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::CHASE);
	UAIBlueprintHelperLibrary::SimpleMoveToActor(controller, target);
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

	const FMonsterInfo& monsterInfo = monster->GetMonsterInfo();
	UMonsterAnimInstance* monsterAnimInst = monster->GetMonsterAnimInst();
	if (!IsValid(monsterAnimInst))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ACharacter* target = Cast<ACharacter>(controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	
	//---------------
	// Target 이 없으면 Idle -> Task 종료.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	//---------------
	// Target 있고, 공격 범위 안에 있으면 공격 시작.
	// Target 있고, 공격 범위 밖에 있으면 추격.
	//---------------
	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();
	monsterPosition -= FVector(0.f, 0.f, monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	targetPosition -= FVector(0.f, 0.f, target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	float	distance = FVector::Distance(monsterPosition, targetPosition);

	distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	distance -= target->GetCapsuleComponent()->GetScaledCapsuleRadius();
	if (distance <= monsterInfo.attackDistance)
	{
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::CHASE);
	UAIBlueprintHelperLibrary::SimpleMoveToActor(controller, target);
}

void UBTTask_TargetTrace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
