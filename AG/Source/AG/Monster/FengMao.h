// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "FengMao.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AFengMao : public AMonster
{
	GENERATED_BODY()
	
public:
	AFengMao();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();
};
