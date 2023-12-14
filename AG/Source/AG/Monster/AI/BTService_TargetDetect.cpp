// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TargetDetect.h"
#include "../MonsterAIController.h"
#include "../Monster.h"


UBTService_TargetDetect::UBTService_TargetDetect()
{
	NodeName = TEXT("TargetDetect");

	// 0.5 �� �������� ó��.
	Interval = 0.3f;

	// 0.5 +- 0.1.
	// �� ���� ������ �ð��� ���ؼ� �� �ð��� �� ���� Tick() ȣ��.
	RandomDeviation = 0.1f;
}

void UBTService_TargetDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//---------------
	// MonsterAIController, Monster, MonsterInfo �� ���´�.
	//---------------

	// ���͵��� �ϳ��� BT �� �����ؼ� ����Ѵ�.
	// -> OwnerComp �� ���͵��� ������ �� �ִ�.
	// -> ��ü(����)���� OwnerComp �� �ϳ��� ���������� ���´�.
	AMonsterAIController* controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(controller))
		return;


	AMonster* monster = Cast<AMonster>(controller->GetPawn());

	if (!IsValid(monster))
		return;

	
	const FMonsterInfo& monsterInfo = monster->GetMonsterInfo();



	//---------------
	// monsterInfo.traceDistance �ȿ� ��ü�� �ִ��� Ȯ���Ѵ�.
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

	//DrawDebugSphere(GetWorld(), monster->GetActorLocation(),	// ���� �߽�.
	//	monsterInfo.traceDistance,								// �Ÿ�.
	//	20,														// �� �������� �� ���ΰ�.
	//	drawColor,												// ����.
	//	false,
	//	0.3f);													// 0.3�ʵ��� �ִٰ� �����.
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
