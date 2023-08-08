// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "ParticleNiagara.h"
#include "ValkyrieBlinkFire.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieBlinkFire : public AParticleNiagara
{
	GENERATED_BODY()

public:
	AValkyrieBlinkFire();

	virtual void Tick(float DeltaTime) override;

	virtual void OnSystemFinish(class UNiagaraComponent* PSystem) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCollisionActor;
};
