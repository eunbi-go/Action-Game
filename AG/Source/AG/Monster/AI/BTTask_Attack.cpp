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
	// Target 이 없으면 Idle, 있으면 Target 을 공격한다.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();


		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		return EBTNodeResult::Failed;
	}

	// else.
	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::ATTACK);


	// 몬스터가 타겟에 도착할 때까지 이 Task를 빠져나가지 못하게 한다.
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
	// MonsterAIController, Monster(Info, AnimInst), BB의 Target 을 얻어온다.
	//---------------
	AMonsterAIController* controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(controller))
	{
		// FinishLatentTask(): Task를 강제로 종료시킨다.
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
	// Target 이 없으면 Idle/Task 종료한다.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		// Task를 종료시킨다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}


	//if (controller->GetBlackboardComponent()->GetValueAsBool(TEXT("IsSkillEnable")))
	//{
	//	monster->SetIsAttackEnd(false);
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	//	return;
	//}

	//---------------
	// Target 이 존재하면 공격이 끝났는지 체크한 후, 계속 공격할지 결정한다.
	//---------------
	//if (monster->GetIsAttackEnd())
	//{
		bool isSkillEnable = controller->GetBlackboardComponent()->GetValueAsBool(TEXT("IsSkillEnable"));

		if (isSkillEnable)
		{
			monster->SetIsAttackEnd(false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

			return;
		}

		FVector monsterPosition = monster->GetActorLocation();
		FVector targetPosition = target->GetActorLocation();

		monsterPosition -= FVector(0.f, 0.f, monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		targetPosition -= FVector(0.f, 0.f, target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		// 두 위치 사이의 거리를 구해준다.
		float	distance = FVector::Distance(monsterPosition, targetPosition);

		// 두 위치 사이의 거리에서 Capsule의 반경을 뺀다.
		distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
		distance -= target->GetCapsuleComponent()->GetScaledCapsuleRadius();

		

		// - Target 이 공격거리 밖으로 벗어나면 공격을 끝낸다.
		if (distance >= monsterInfo.attackDistance)
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		// - 공격을 한다. 하지만 이때, 몬스터가 이동을 멈추고, Target 을 바라보도록 한다.
		else
		{
			//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("attack start distance: %f"), distance));
			
			controller->StopMovement();

			FVector direction = targetPosition - monsterPosition;
			FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

			monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 2.f));

			monsterAnimInst->Attack();
		}
	//}

}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}