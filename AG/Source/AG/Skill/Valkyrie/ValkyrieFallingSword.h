// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGSkillActor.h"
#include "ValkyrieFallingSword.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieFallingSword : public AAGSkillActor
{
	GENERATED_BODY()
public:
	AValkyrieFallingSword();
	virtual void Activate() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnEffect() override;
	virtual void SkillEnd() override;

	UFUNCTION()
	void ParticleFinish(class UNiagaraComponent* PSystem);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCollisionActor;
};
