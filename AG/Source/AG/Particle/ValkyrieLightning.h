// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "ParticleNiagara.h"
#include "ValkyrieLightning.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieLightning : public AParticleNiagara
{
	GENERATED_BODY()

public:
	AValkyrieLightning();

	bool GetIsContinue() { return true; }

	virtual void OnSystemFinish(class UNiagaraComponent* PSystem) override;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnCollisionActor();

private:
	FTimerHandle	mTimer;

	bool isContinue = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCollisionActor;
};
