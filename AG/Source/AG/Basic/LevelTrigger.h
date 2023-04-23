// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerActor.h"
#include "LevelTrigger.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ALevelTrigger : public ATriggerActor
{
	GENERATED_BODY()

public:
	ALevelTrigger();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FName	mLevelName;

public:
	virtual void TriggerBegin(const FHitResult& SweepResult);
	virtual void TriggerEnd();
};
