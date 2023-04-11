// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"


UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	// Target �� ������ Idle, ������ Target �� �����Ѵ�.
	//---------------
	if (!IsValid(target))
	{
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		return EBTNodeResult::Failed;
	}

	// else.
	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::ATTACK);


	// ���Ͱ� Ÿ�ٿ� ������ ������ �� Task�� ���������� ���ϰ� �Ѵ�.
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	// Target �� ������ Idle/Task �����Ѵ�.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		// Task�� �����Ų��.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}



	//---------------
	// Target �� �����ϸ� ������ �������� üũ�� ��, ��� �������� �����Ѵ�.
	//---------------
	if (monster->GetIsAttackEnd())
	{
		bool isSkillEnable = controller->GetBlackboardComponent()->GetValueAsBool(TEXT("IsSkillEnable"));
		
		if (isSkillEnable)
		{
			monster->SetIsAttackEnd(false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

			return;
		}

		FVector monsterPosition = monster->GetActorLocation();
		FVector targetPosition = target->GetActorLocation();

		float distance = (monsterPosition - targetPosition).Size();


		// - Target �� ���ݰŸ� ������ ����� ������ ������.
		if (distance >= monsterInfo.attackDistance)
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		// - ������ �Ѵ�. ������ �̶�, ���Ͱ� �̵��� ���߰�, Target �� �ٶ󺸵��� �Ѵ�.
		else
		{
			controller->StopMovement();

			FVector direction = targetPosition - monsterPosition;
			FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

			monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 10.f));
		}

		//monsterAnimInst->Attack();
		monster->SetIsAttackEnd(false);
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}