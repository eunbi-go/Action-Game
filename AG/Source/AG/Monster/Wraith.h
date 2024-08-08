// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Wraith.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AWraith : public AMonster
{
	GENERATED_BODY()
	
public:
	AWraith();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeDefaultAttributes() const;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();
	
public:
	virtual void NormalAttackCheck();
};
