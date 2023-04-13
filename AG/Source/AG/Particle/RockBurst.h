// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleCascade.h"
#include "RockBurst.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ARockBurst : public AParticleCascade
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReSpawn, class ARockBurst*, niagara);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);

public:
	ARockBurst();

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION()
		void ParticleColiision(FName EventName, float EmitterTime, int32 ParticleTime,
			FVector Location, FVector Velocity, FVector Direction,
			FVector Normal, FName BoneName, class UPhysicalMaterial* PhysMat);

	UFUNCTION()
	void ParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void Spawn();

	UPROPERTY(BlueprintAssignable)
	FOnReSpawn	mReSpawn;

	UPROPERTY(BlueprintAssignable)
	FOnHit	mOnHit;

protected:
	UPROPERTY()
	FTimerHandle mTimerHandle;
};
