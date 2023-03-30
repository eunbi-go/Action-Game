// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "TeleportSkill.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ATeleportSkill : public ASkillActor
{
	GENERATED_BODY()
	
public:
	ATeleportSkill();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void CollisionProjectile(const FHitResult& Hit);
};
