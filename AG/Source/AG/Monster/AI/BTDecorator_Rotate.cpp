// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Rotate.h"

#include "../MonsterAIController.h"
#include "../Monster.h"


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
	//---------------

	FVector monsterPosition = monster->GetActorLocation();
	FVector targetPosition = target->GetActorLocation();

	//monster->SetRot(true);
	monster->SetTargetPos(targetPosition);

	float distance = (monsterPosition - targetPosition).Size();

	FVector direction = targetPosition - monsterPosition;
	FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 2.f));

	// 회전각 구하기.
	direction = targetPosition - monsterPosition;
	direction.Z = 0.f;
	direction.Normalize();

	float innerProduct = FVector::DotProduct(monster->GetActorForwardVector(), direction);
	float degree = UKismetMathLibrary::DegAcos(innerProduct);

	FVector outProduct = FVector::CrossProduct(monster->GetActorForwardVector(), direction);
	float sign = UKismetMathLibrary::SignOfFloat(outProduct.Z);

	float returnDegree = sign * degree;

	bool returnValue = false;

	if ((float)FMath::Abs((float)returnDegree) < 190.f)
	{
		returnValue = true;
		
	}

	return returnValue;
}
