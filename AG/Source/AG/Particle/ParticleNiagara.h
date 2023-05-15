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

protected:
	virtual void BeginPlay() override;


public:
	virtual void SetParticle(UNiagaraSystem* _particle);
	virtual void SetParticle(const FString& _path);



public:
	UNiagaraComponent* GetNiagara() { return mParticle; }



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mParticle;
};
