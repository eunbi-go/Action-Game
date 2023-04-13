// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleCascade.h"
#include "RockBurst.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ARockBurst : public AParticleCascade
{
	GENERATED_BODY()
	
public:
	ARockBurst();

protected:
	virtual void BeginPlay() override;
};
