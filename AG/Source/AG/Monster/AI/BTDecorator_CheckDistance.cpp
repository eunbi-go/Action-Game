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
	// MonsterAIController, Monster, MonsterInfo, BB의 Target 을 얻어온다.
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
	// 스킬 사용중이면 체크X (스킬 사용(시작/끝 부분 제외) 중 회전 못하도록)
	//---------------
	if (!monsterAnimInst->GetIsSkillEnd())
	{
			return false;
	}

	//---------------
	// 몬스터와 타겟의 거리를 구해 attack/trace Distance 와 비교한다.
	//---------------
	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();
	monsterPosition -= FVector(0.f, 0.f, monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	targetPosition -= FVector(0.f, 0.f, target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	float	distance = FVector::Distance(monsterPosition, targetPosition);
	distance -= monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	distance -= target->GetCapsuleComponent()->GetScaledCapsuleRadius();

	float checkDistance = 0.f;
	bool check_skill = false;
	switch (mCheckType)
	{
	case CHECK_TYPE::ATTACK:
		checkDistance = monsterInfo.attackDistance;
		break;

	case CHECK_TYPE::TRACE:
		checkDistance = monsterInfo.traceDistance;
		break;

	case CHECK_TYPE::Skill:
		check_skill = monster->CheckEnableSkill();
		if (check_skill && monsterAnimInst->GetIsSkillEnd())
			return true;
		return false;
		break;
	}

	
	return distance <= checkDistance;
}
