// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"

#include "../MonsterAIController.h"
#include "../Monster.h"


UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//---------------
	// MonsterAIController, Monster, MonsterInfo, BB�� Target �� ���´�.
	//---------------
	AMonsterAIController* controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(controller))
		return false;


	AMonster* monster = Cast<AMonster>(controller->GetPawn());

	if (!IsValid(monster))
		return false;

	ACharacter* target = Cast<ACharacter>(controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	
	if (!IsValid(target))
		return false;


	const FMonsterInfo& monsterInfo = monster->GetMonsterInfo();



	//---------------
	// ���Ϳ� Ÿ���� �Ÿ��� ���� attack/trace Distance �� ���Ѵ�.
	//---------------
	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();
	
	monsterPosition -= FVector(0.f, 0.f, monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	targetPosition -= FVector(0.f, 0.f, target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// �� ��ġ ������ �Ÿ��� �����ش�.
	float	distance = FVector::Distance(monsterPosition, targetPosition);

	// �� ��ġ ������ �Ÿ����� Capsule�� �ݰ��� ����.
	distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	distance -= target->GetCapsuleComponent()->GetScaledCapsuleRadius();

	float checkDistance = 0.f;

	switch (mCheckType)
	{
	case CHECK_TYPE::ATTACK:
		checkDistance = monsterInfo.attackDistance;
		break;

	case CHECK_TYPE::TRACE:
		checkDistance = monsterInfo.traceDistance;
		break;
	}

	return distance <= checkDistance;
}
