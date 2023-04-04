// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "SlashSkill.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ASlashSkill : public ASkillActor
{
	GENERATED_BODY()

public:
	ASlashSkill();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void Finish(UNiagaraComponent* comp);
};
