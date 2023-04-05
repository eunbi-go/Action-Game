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
		// ai컨트롤러가 사용하는 인공지능 시스템으로 지정한다.
		//-------------

		UBlackboardComponent* blackboardRef = Blackboard;

		// 블랙보드가 사용할 수 있는 경우에만 AiTree를 동작시킨다.
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
