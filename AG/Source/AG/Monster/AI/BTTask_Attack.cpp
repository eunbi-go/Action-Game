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
	// (Target �� ������) || (������̴� ��ų�� ���� ������ �ʾ�����) Task �����Ѵ�.
	//---------------
	if (!IsValid(target) || !monsterAnimInst->GetIsSkillEnd())
	{
		controller->StopMovement();
		return EBTNodeResult::Failed;
	}

	controller->StopMovement();
	mIsAttacking = true;
	monster->SetIsAttackEnd(false);

	monster->mOnAttackEnd.AddLambda([this, &monster]()-> void {
		mIsAttacking = false;
		//monster->SetIsAttackEnd(true);
		});
	monsterAnimInst->Attack();


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
	// (Target �� ������) Task �����Ѵ�.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//---------------
	// ������ �������� üũ�� ��, ��� �������� �����Ѵ�.
	//---------------
	if (mIsAttacking)
		return;
	
	// ���� ����
	controller->StopMovement();

	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();

	monsterPosition -= FVector(0.f, 0.f, monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	targetPosition -= FVector(0.f, 0.f, target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// �� ��ġ ������ �Ÿ��� �����ش�.
	float	distance = FVector::Distance(monsterPosition, targetPosition);

	// �� ��ġ ������ �Ÿ����� Capsule�� �ݰ��� ����.
	distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	distance -= target->GetCapsuleComponent()->GetScaledCapsuleRadius();



	// (Target �� ���ݰŸ� ������ �����) || (��ų�� ����� �� ������) ������ ������.
	if (distance >= monsterInfo.attackDistance || monster->IsEnableUseSkill())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		mIsAttacking = true;
		monster->SetIsAttackEnd(false);
		monsterAnimInst->Attack();
	}

	
	
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}