// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToTarget.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"


UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
	NodeName = TEXT("RotateToTarget");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	// Target �� ������ Idle.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);
		return EBTNodeResult::Failed;
	}

	//---------------
	// Target �� ������ Target �� ���� ȸ���Ѵ�.
	//---------------
	controller->StopMovement();
	FVector direction = target->GetActorLocation() - monster->GetActorLocation();
	direction.Z = 0.f;
	FRotator targetRotation = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();
	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), targetRotation, GetWorld()->GetDeltaSeconds(), 2.f));

	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_RotateToTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_RotateToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
