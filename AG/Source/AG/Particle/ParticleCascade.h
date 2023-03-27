// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleActor.h"
#include "ParticleCascade.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AParticleCascade : public AParticleActor
{
	GENERATED_BODY()
	
public:
	AParticleCascade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void SetParticle(UParticleSystem* _particle);
	virtual void SetParticle(const FString& _path);



public:
	UParticleSystemComponent* GetParticle() { return mParticle; }



public:
	UFUNCTION()
	void ParticleFinish(UParticleSystemComponent* _particle);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UParticleSystemComponent* mParticle;
};
