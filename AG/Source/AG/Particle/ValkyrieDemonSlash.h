// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "ParticleNiagara.h"
#include "ValkyrieDemonSlash.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieDemonSlash : public AParticleNiagara
{
	GENERATED_BODY()

public:
	AValkyrieDemonSlash();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FTimerHandle	mTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCollisionActor;
};
