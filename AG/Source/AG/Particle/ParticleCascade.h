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
	virtual void BeginPlay() override;

public:
	virtual void SetParticle(UParticleSystem* _particle);
	virtual void SetParticle(const FString& _path);

	void SpawnAtLocation(const FVector& _location);

public:
	UParticleSystemComponent* GetParticle()
	{
		return mParticle; 
	}



public:
	UFUNCTION()
	void ParticleFinish(UParticleSystemComponent* _particle);
	


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UParticleSystemComponent* mParticle;
};
