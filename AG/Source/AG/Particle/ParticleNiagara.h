// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleActor.h"
#include "ParticleNiagara.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AParticleNiagara : public AParticleActor
{
	GENERATED_BODY()
	

public:
	AParticleNiagara();

	UFUNCTION()
	virtual void OnSystemFinish(class UNiagaraComponent* PSystem);
	virtual void SpawnHitEffect(const FVector& Location, const FRotator& Rotator) override;

protected:
	virtual void BeginPlay() override;


public:
	virtual void SetParticle(UNiagaraSystem* _particle);
	virtual void SetParticle(const FString& _path);
	void SetNiagaraScale(const FVector& _scale);
	

public:
	UNiagaraComponent* GetNiagara() 
	{
		return mParticle; 
	}



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mParticle;
};
