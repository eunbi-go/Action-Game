// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"


UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	// Target 이 있거나 몬스터가 순찰할 수 없는 상태이면 종료시킨다.
	//---------------
	if (IsValid(target) || !monster->GetIsPatrolEnable())
		return EBTNodeResult::Succeeded;

	

	//---------------
	// 몬스터 순찰을 시작한다.
	//---------------
	
	const FMonsterInfo& info = monster->GetMonsterInfo();

	monster->GetCharacterMovement()->MaxWalkSpeed = info.movingWalkSpeed;
	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::PATROL);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller,
		monster->GetPatrolPosition());


	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//---------------
	// 몬스터가 Patrol Point 에 도착했는지 확인한다.
	// 도착했으면 몬스터가 다음 Patrol Point 로 순찰할 수 있게 해준다.
	//---------------

	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = monster->GetPatrolPosition();
	monsterPosition.Z = targetPosition.Z = 0.f;

	float distance = FVector::Distance(monsterPosition, targetPosition);
	distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (distance <= 10.f)
	{
		controller->StopMovement();
		monster->GoNextPatrolPoint();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Patrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
