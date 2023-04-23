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
	bool isSkillEnable = controller->GetBlackboardComponent()->GetValueAsBool(TEXT("IsSkillEnable"));


	//---------------
	// Target 이 없으면 Idle, 있으면 Target 을 공격한다.
	//---------------
	if (!IsValid(target) || !isSkillEnable)
	{
		controller->StopMovement();

		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		return EBTNodeResult::Failed;
	}

	// else.
	const FMonsterSkillInfo* info = monster->GetSkillInfo();

	if (info != nullptr)
	{
		monsterAnimInst->SetMonsterMotionType(info->animType);

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
	bool isSkillEnable = controller->GetBlackboardComponent()->GetValueAsBool(TEXT("IsSkillEnable"));

	if (!IsValid(controller) || !isSkillEnable)
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

	if (!IsValid(target))
	{
		PrintViewport(3.f, FColor::Green, TEXT("no target"));
		controller->StopMovement();

		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}



	if (monsterAnimInst->GetIsSkillEnd())
	{
		const FMonsterInfo& info = monster->GetMonsterInfo();
		float capsuleHalfHeight = Cast<ACharacter>(target)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		
		FVector targetPosition = target->GetActorLocation();
		targetPosition.Z -= capsuleHalfHeight;

		FVector position = monster->GetActorLocation();
		position.Z -= monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		float distance = (float)FVector::Distance(targetPosition, position);
		distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
		distance -= Cast<ACharacter>(target)->GetCapsuleComponent()->GetScaledCapsuleRadius();


		// - Target 이 공격거리 밖으로 벗어나면 공격을 끝낸다.
		if (distance >= info.attackDistance)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			//return;
		}

		else
		{
			controller->StopMovement();

			FVector direction = target->GetActorLocation() - monster->GetActorLocation();
			FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

			monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 10.f));


			const FMonsterSkillInfo* skillInfo = monster->GetSkillInfo();
			if (skillInfo != nullptr)
			{
				monsterAnimInst->SetMonsterMotionType(skillInfo->animType);
				monsterAnimInst->SetIsSkillEnd(false);
			}
			else
			{
				controller->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSkillEnable"), false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

			}

			
		}

	}



}

void UBTTask_MonsterSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
