// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Khaimera.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AKhaimera : public AMonster
{
	GENERATED_BODY()
	
public:
	AKhaimera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void NormalAttackCheck();
};


