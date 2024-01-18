// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterSkill.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"


UBTTask_MonsterSkill::UBTTask_MonsterSkill()
{
	NodeName = TEXT("MonsterSkill");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MonsterSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	// (Target 이 없으면) || (현재 스킬을 사용할 수 없으면) 종료, 있으면 Target 을 공격한다.
	//---------------
	if (!IsValid(target) || !monster->IsEnableUseSkill())
	{
		controller->StopMovement();
		return EBTNodeResult::Failed;
	}



	// else.
	FVector direction = target->GetActorLocation() - monster->GetActorLocation();
	FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();
	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 10.f));


	const FMonsterSkillInfo* info = monster->GetSkillInfo();

	if (monster->CheckEnableSkill())
	{
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::SKILL1);
	}
	else
	{
		controller->StopMovement();
		return EBTNodeResult::Failed;

	}

	// 몬스터가 타겟에 도착할 때까지 이 Task를 빠져나가지 못하게 한다.
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_MonsterSkill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_MonsterSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//---------------
	// MonsterAIController, Monster, BB의 Target 을 얻어온다.
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

	//---------------
	// (Target 이 없으면) || (현재 스킬을 사용할 수 없으면) 종료, 있으면 Target 을 공격한다.
	//---------------
	if (!IsValid(target) || !monster->IsEnableUseSkill())
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	controller->StopMovement();

	bool isAtkEnd = monster->GetIsAttackEnd();
	bool isSkillEnd = monsterAnimInst->GetIsSkillEnd();
	if (isAtkEnd && isSkillEnd)
	{
		if (monster->CheckEnableSkill())
		{
			monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::SKILL1);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
		
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}


}

void UBTTask_MonsterSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
