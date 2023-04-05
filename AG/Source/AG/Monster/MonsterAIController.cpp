// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (IsValid(mAiTree) && IsValid(mAiBlackboard))
	{
		//-------------
		// ai��Ʈ�ѷ��� ����ϴ� �ΰ����� �ý������� �����Ѵ�.
		//-------------

		UBlackboardComponent* blackboardRef = Blackboard;

		// �����尡 ����� �� �ִ� ��쿡�� AiTree�� ���۽�Ų��.
		if (UseBlackboard(mAiBlackboard, blackboardRef))
		{
			RunBehaviorTree(mAiTree);
		}
	}
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMonsterAIController::SetBehaviorTree(const FString& _tree)
{
	mAiTree = LoadObject<UBehaviorTree>(nullptr, *_tree);
}

void AMonsterAIController::SetBlackboard(const FString& _blackboard)
{
	mAiBlackboard = LoadObject<UBlackboardData>(nullptr, *_blackboard);
}
