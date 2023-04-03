// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "SprintSkil.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ASprintSkil : public ASkillActor
{
	GENERATED_BODY()

public:
	ASprintSkil();


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);


public:
	virtual void Tick(float DeltaTime) override;


public:
	void SetTarget(class APlayerCharacter* _target) { mTarget = _target; }

protected:
	class APlayerCharacter* mTarget;
};
