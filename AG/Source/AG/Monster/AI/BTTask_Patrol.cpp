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
	if (IsValid(target) || !monster->GetPatrolEnable())
		return EBTNodeResult::Succeeded;

	

	//---------------
	// 몬스터 순찰을 시작한다.
	//---------------
	const FMonsterInfo& info = monster->GetMonsterInfo();

	PrintViewport(3.f, FColor::Orange, TEXT("Patrol Start"));

	monster->GetCharacterMovement()->MaxWalkSpeed = info.movingWalkSpeed;
	monsterAnimInst->SetMonsterMotionType(MONSTER_MOTION::PATROL);

	monster->SetIsPatrolEnable(true);


	//// 회전.
	//FVector monsterPosition = monster->GetActorLocation();
	//FVector targetPosition = monster->GetPatrolPosition();
	//FVector direction = targetPosition - monsterPosition;

	//FRotator targetRotation = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

	//monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), targetRotation, GetWorld()->GetDeltaSeconds(), 10.f));

	// 이동.
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
		monster->SetIsPatrolEnable(false);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector direction = monster->GetPatrolPosition() - monster->GetActorLocation();

	FRotator targetRotation = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

	//monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), targetRotation, GetWorld()->GetDeltaSeconds(), 2.f));


	//---------------
	// PatrolType (SPLINE / POINT) 에 따라 다음 Patro Point 를 구한다.
	//---------------

	if (monster->GetPatrolType() == PATROL_TYPE::SPLINE)
	{
		//---------------
		// 몬스터가 Patrol Point 에 도착했는지 확인한다.
		// 도착했으면 몬스터가 다음 Patrol Point 로 순찰할 수 있게 해준다.
		//---------------




		UAIBlueprintHelperLibrary::SimpleMoveToLocation(
			controller,
			monster->GetPatrolPosition());

		FVector	MonsterLoc = monster->GetActorLocation();

		MonsterLoc = MonsterLoc -
			FVector(0.f, 0.f, monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		float	Distance = FVector::Distance(MonsterLoc, monster->GetPatrolPointPosition());

		Distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (Distance <= 10.f)
		{
			//PrintViewport(5.f, FColor::Red, FString::Printf(TEXT("%d to next"), monster->GetPatrolIndex()));

			monster->SetIsPatrolEnable(false);
			controller->StopMovement();
			monster->GoNextPatrolPoint();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}



	else if (monster->GetPatrolType() == PATROL_TYPE::POINT)
	{
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
			monster->SetIsPatrolEnable(false);
			controller->StopMovement();
			monster->GoNextPatrolPoint();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

void UBTTask_Patrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
