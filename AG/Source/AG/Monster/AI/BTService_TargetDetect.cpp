// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TargetDetect.h"
#include "../MonsterAIController.h"
#include "../Monster.h"


UBTService_TargetDetect::UBTService_TargetDetect()
{
	NodeName = TEXT("TargetDetect");

	// 0.5 초 간격으로 처리.
	Interval = 0.3f;

	// 0.5 +- 0.1.
	// 이 사이 랜덤한 시간을 정해서 그 시간에 한 번씩 Tick() 호출.
	RandomDeviation = 0.1f;
}

void UBTService_TargetDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//---------------
	// MonsterAIController, Monster, MonsterInfo 를 얻어온다.
	//---------------

	// 몬스터들은 하나의 BT 를 공유해서 사용한다.
	// -> OwnerComp 로 몬스터들을 구분할 수 있다.
	// -> 객체(몬스터)마다 OwnerComp 는 하나씩 독립적으로 들어온다.
	AMonsterAIController* controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(controller))
		return;


	AMonster* monster = Cast<AMonster>(controller->GetPawn());

	if (!IsValid(monster))
		return;

	
	const FMonsterInfo& monsterInfo = monster->GetMonsterInfo();



	//---------------
	// monsterInfo.traceDistance 안에 객체가 있는지 확인한다.
	//---------------


	TArray<FOverlapResult>	resultArray;
	FCollisionQueryParams	param(NAME_None, false, monster);

	bool isCollision = GetWorld()->OverlapMultiByChannel(
						resultArray,
						monster->GetActorLocation(),	
						FQuat::Identity,
						ECollisionChannel::ECC_GameTraceChannel5,
						FCollisionShape::MakeSphere(monsterInfo.traceDistance),
						param);

#if ENABLE_DRAW_DEBUG 
	//FColor drawColor = isCollision ? FColor::Red : FColor::Green;

	//DrawDebugSphere(GetWorld(), monster->GetActorLocation(),	// 원의 중심.
	//	monsterInfo.traceDistance,								// 거리.
	//	20,														// 몇 조각으로 낼 것인가.
	//	drawColor,												// 색상.
	//	false,
	//	0.3f);													// 0.3초동안 있다가 사라짐.
#endif

	if (isCollision)
	{
		controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), resultArray[0].GetActor());
	}
	
	else
	{
		monster->ClearAllSkill();
		controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
}
