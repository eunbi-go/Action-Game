// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

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

	UMonsterAnimInstance* monsterAnimInst = monster->GetMonsterAnimInst();

	if (!IsValid(monsterAnimInst))
	{
		return false;
	}


	//---------------
	// ��ų ������̸� üũX (��ų ���(����/�� �κ� ����) �� ȸ�� ���ϵ���)
	//---------------
	if (!monsterAnimInst->GetIsSkillEnd())
	{
			return false;
	}

	//---------------
	// ���Ϳ� Ÿ���� �Ÿ��� ���� attack/trace Distance �� ���Ѵ�.
	//---------------
	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();
	monsterPosition -= FVector(0.f, 0.f, monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	targetPosition -= FVector(0.f, 0.f, target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	float	distance = FVector::Distance(monsterPosition, targetPosition);
	distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	distance -= target->GetCapsuleComponent()->GetScaledCapsuleRadius();

	float checkDistance = 0.f;
	bool ch = false;
	switch (mCheckType)
	{
	case CHECK_TYPE::ATTACK:
		checkDistance = monsterInfo.attackDistance;
		break;

	case CHECK_TYPE::TRACE:
		checkDistance = monsterInfo.traceDistance;
		break;

	case CHECK_TYPE::Skill:
		ch = monster->CheckEnableSkill();
		if (ch && monsterAnimInst->GetIsSkillEnd())
			checkDistance = 5000.f;
		else
			checkDistance = 0.f;
		break;
	}

	
	return distance <= checkDistance;
}
