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
	// Target 이 없으면 Idle.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		return EBTNodeResult::Failed;
	}



	//---------------
	// Target 이 있으면 Target 을 향해 회전한다.
	//---------------
	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();
	FVector direction = targetPosition - monsterPosition;

	FRotator targetRotation = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), targetRotation, GetWorld()->GetDeltaSeconds(), 2.f));



	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_RotateToTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	// Target 이 없으면 Idle/Task 종료.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		// Task를 종료시킨다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}



	//---------------
	// Target 이 있으면 Target 을 향해 회전한 후,
	//---------------
	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();
	FVector direction = targetPosition - monsterPosition;
	FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 2.f));

	//---------------
	// Target 과의 각도가 일정 각도 이하가 되면 Target을 향해 바라보는 회전을 끝냈다고 판단 / Task 종료.
	//---------------
	direction = targetPosition - monsterPosition;
	direction.Z = 0.f;
	direction.Normalize();

	float innerProduct = FVector::DotProduct(monster->GetActorForwardVector(), direction);
	float degree = UKismetMathLibrary::DegAcos(innerProduct);

	FVector outProduct = FVector::CrossProduct(monster->GetActorForwardVector(), direction);
	float sign = UKismetMathLibrary::SignOfFloat(outProduct.Z);

	float angle = sign * degree;

	
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("angle: %f"), (float)FMath::Abs((float)angle)));
	
	bool returnValue = false;

	if (sign <= 0.f)
		returnValue = true;
	else
		returnValue = false;

	if (degree <= 10.f)
		returnValue = true;
	else
		returnValue = false;

	if (!returnValue)
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//return;
	}
		
}

void UBTTask_RotateToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
