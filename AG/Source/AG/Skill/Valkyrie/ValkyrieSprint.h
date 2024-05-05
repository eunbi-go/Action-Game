// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGSkillActor.h"
#include "ValkyrieSprint.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieSprint : public AAGSkillActor
{
	GENERATED_BODY()
	
public:
	AValkyrieSprint();
	virtual void Activate() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnEffect() override;
	virtual void FindTarget() override;
	virtual void SkillEnd() override;



protected:
	virtual void BeginPlay() override;

	FVector mTargetLocation = FVector();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCollisionActor;
};
