// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleNiagara.h"
#include "RampageSlash.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ARampageSlash : public AParticleNiagara
{
	GENERATED_BODY()
	
public:
	ARampageSlash();

	UFUNCTION()
	void Stop();

	UFUNCTION()
		void Start();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		FTimerHandle mTimerHandle2;
};
