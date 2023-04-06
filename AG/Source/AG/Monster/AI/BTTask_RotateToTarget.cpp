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
	// Target �� ������ Idle, ������ Target �� �Ѿư���.
	//---------------
	if (!IsValid(target))
	{
		controller->StopMovement();
		monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);

		return EBTNodeResult::Failed;
	}

	// else.
	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::IDLE);
	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();

	float distance = (monsterPosition - targetPosition).Size();

	FVector direction = targetPosition - monsterPosition;
	FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 10.f));

	// ���Ͱ� Ÿ�ٿ� ������ ������ �� Task�� ���������� ���ϰ� �Ѵ�.
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

	FVector direction = targetPosition - monsterPosition;
	FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();
	
	FRotator src = monster->GetActorRotation().GetNormalized();

	monster->SetActorRotation(FMath::RInterpTo(src, rot, DeltaSeconds, 10.f));
	
	// ȸ���� ���ϱ�.
	direction = targetPosition - monsterPosition;
	direction.Z = 0.f;
	direction.Normalize();

	float innerProduct = FVector::DotProduct(monster->GetActorForwardVector(), direction);
	float degree = UKismetMathLibrary::DegAcos(innerProduct);

	FVector outProduct = FVector::CrossProduct(monster->GetActorForwardVector(), direction);
	float sign = UKismetMathLibrary::SignOfFloat(outProduct.Z);

	float returnDegree = sign * degree;

	
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("angle: %f"), (float)FMath::Abs((float)returnDegree)));
	
	if ((float)FMath::Abs((float)returnDegree) < 10.f)
	{
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_RotateToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
