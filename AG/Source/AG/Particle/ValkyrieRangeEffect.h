// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "ParticleNiagara.h"
#include "ValkyrieRangeEffect.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieRangeEffect : public AParticleNiagara
{
	GENERATED_BODY()

public:
	AValkyrieRangeEffect();
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnHitEffect(const FVector& Location, const FRotator& Rotator) override;
	virtual void OnSystemFinish(class UNiagaraComponent* PSystem) override;

	void SetCollisionSize(const FVector& Size);
	void SetCollisionRotate(const FRotator& Rotator);



protected:
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCollisionActor;

	FVector mCollisionSize;
	FRotator mCollisionRotator;
	bool mIsInitCollisionActor = false;

};
