// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Rotate.h"

#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"


UBTDecorator_Rotate::UBTDecorator_Rotate()
{
}

bool UBTDecorator_Rotate::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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



	//---------------
	// 몬스터가 Target 을 향해 회전한다.
	//---------------

	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();
	FVector direction = targetPosition - monsterPosition;

	FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 2.f));
	monster->GetMonsterAnimInst()->SetMonsterMotionType(MONSTER_MOTION::IDLE);

	//---------------
	// 몬스터와 Target 사이의 각도를 구해 일정 각도 이상이라면 return true-> 자식 노드 실행.
	//---------------
	direction = targetPosition - monsterPosition;
	direction.Z = 0.f;
	direction.Normalize();

	float innerProduct = FVector::DotProduct(monster->GetActorForwardVector(), direction);
	float degree = UKismetMathLibrary::DegAcos(innerProduct);

	FVector outProduct = FVector::CrossProduct(monster->GetActorForwardVector(), direction);
	float sign = UKismetMathLibrary::SignOfFloat(outProduct.Z);

	float angle = sign * degree;
	
	//PrintViewport(1.f, FColor::Yellow, FString::Printf(TEXT("angle: %f"), (float)FMath::Abs((float)angle)));
	
	bool returnValue = false;

	//if (sign <= 0.f)
	//	returnValue = true;
	//else
	//	returnValue = false;

	if (degree <= 10.f)
		returnValue = true;
	else
		returnValue = false;

	//if (!returnValue)
	//	controller->StopMovement();

	return returnValue;
}
