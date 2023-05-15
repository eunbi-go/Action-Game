// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleNiagara.h"
#include "DemonSlash.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ADemonSlash : public AParticleNiagara
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHit, class AActor*, hitActor, const FHitResult&, hit);

public:
	ADemonSlash();
	
	UPROPERTY(BlueprintAssignable)
	FOnHit	mHitDelegate;


protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void ParticleFinish(UNiagaraComponent* _particle);
	
	UFUNCTION()
	void ParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
