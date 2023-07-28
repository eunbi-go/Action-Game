// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "ParticleNiagara.h"
#include "ValkyrieSlash.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieSlash : public AParticleNiagara
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetDirection(const FVector& _value) { mDirection = _value; }

	void Stop();
	void Start();

private:
	FVector mDirection;

	UPROPERTY()
	FTimerHandle	mTimer;
};
