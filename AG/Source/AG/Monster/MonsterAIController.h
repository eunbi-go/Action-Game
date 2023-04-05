// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"


UCLASS()
class AG_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterAIController();

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;


public:
	void SetBehaviorTree(const FString& _tree);
	void SetBlackboard(const FString& _blackboard);




private:
	// AMonsterAIController 는 BT를 갖고 동작한다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBehaviorTree* mAiTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBlackboardData* mAiBlackboard;
};
