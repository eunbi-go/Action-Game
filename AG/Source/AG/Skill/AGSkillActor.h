// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "AGSkillActor.generated.h"


UCLASS()
class AG_API AAGSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAGSkillActor();
	virtual void Tick(float DeltaTime) override;

	virtual void Activate();
	virtual void SpawnEffect();
	virtual void FindTarget();
	virtual void SkillEnd();
	void SetOwnerActor(AActor* OwnerActor)
	{
		mOwner = OwnerActor;
	}
	
protected:
	virtual void BeginPlay() override;


	UPROPERTY()
	AActor* mOwner;
};
