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
	UFUNCTION()
	void ParticleFinish(UNiagaraComponent* _particle);

	UFUNCTION()
	void ParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Check();
public:
	UNiagaraComponent* GetNiagara() { return mParticle; }



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mParticle;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* MyComp;

	UPROPERTY()
	FTimerHandle timerHandle;

	bool	mIsHit;
	FHitResult	mHitResult;
};
